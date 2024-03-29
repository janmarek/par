#include <iostream>
#include <algorithm>
#include <vector>
#include "CombinationIterator.h"
#include "Graph.h"
#include "Solution.h"
#include "Process.h"
#include <mpi/mpi.h>

using namespace std;

Process::Process(Graph * graph, int processNum, int processCount)
{
	this->graph = graph;
	this->isMaster = processNum == 0;
	this->processCount = processCount;
	this->processNum = processNum;
	iter = 0;
	stopped = false;
	combinationSize = graph->getEdgeCount();
	bestPricePossible = combinationSize % 2;
	myBestPrice = -1;
	sendJobTo = -1;
	sendBest = false;
	vector<ProcessState> prVc(processCount, STATE_WORKING);
	this->processes = prVc;
	setState(STATE_NEW);
}

void Process::run()
{
	int counter = 0;
	
	if (isMaster) {
		cout << "[P0] initializing master (edge count " << combinationSize << ")" << endl;

		CombinationIterator * tmp = new CombinationIterator(
			new EdgeCombination(combinationSize),
			EdgeCombination::createMaxCombination(combinationSize)
		);
		vector<CombinationIterator *> intervals = tmp->divide(processCount);
		delete tmp;

		this->iter = intervals[0];

		for (unsigned int i = 1; i < intervals.size(); ++i) {
			sendJobMessage(i, intervals[i]);
		}

		cout << "[P0] initial jobs sent" << endl;
		setState(STATE_WORKING);
	}
	
	while (true) {
		if (stopped) {
			break;
		}
	
		// iter nemusi existovat u procesu, co jeste nedostal JOB
		if (iter != 0) {
			// zkontroluje aktualni solution a pripadne aktualizuje promenne
			checkSolution(iter->getCurrent()->clone());
			
			counter++;
			
			// umelej spanek pro testovani
			// usleep((((processNum + 1) % 2) + 1) * 500 * 1000);
			
			// message check and send
			if ((isMaster && counter == MASTER_TIMEOUT) || counter == TIMEOUT || !iter->hasNext() || myBestPrice == bestPricePossible) {
				counter = 0;
				checkMessages();
				sendMessages();
			}
	
			// prepni na dalsi
			if (iter->hasNext()) {
				iter->next();
			}
		} else {
			receiveJobMessage();
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD);

	// master ceka na vsechny dilci vysledky a pak vypise solution
	if (isMaster) {
		// print solution
		if (myBestPrice != -1) {
			myBestSolution->print();
		} else {
			cout << "Reseni neexistuje" << endl;
		}
	}
}

bool Process::checkSolution(EdgeCombination * c)
{
	// create solution: graph is shared instance, combination instance is cloned
	Solution * s = new Solution(graph, c);

	// triangle test and price test
    
	if (s->getPrice() <= graph->getTrivialPriceLimit() &&  graph->testTriangleOk(iter->getCurrent()) && (myBestPrice == -1 || myBestPrice > s->getPrice())) {
		// update data
		myBestPrice = s->getPrice();

		cout << "[P" << processNum << "] update best with ";
		s->getCombination()->print();
		cout << " (price " << myBestPrice << ")" << endl;
		sendBest = true;

		myBestSolution = s;

		return true;
	} else {
		// delete current solution instance
		delete s;

		return false;
	}
}

void Process::checkMessages()
{
	MPI_Status status;
	int flag = 0;
	bool first = true;

	do {
		MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

		if (!flag) {
			if (first) {
				usleep(1000);
			}
		} else {
			if (status.MPI_TAG == CMD_NEWBEST) {
				receiveBest(status);
			}

			if (status.MPI_TAG == CMD_STOP) {
				receiveStop(status);
			}

			if (status.MPI_TAG == CMD_JOB) {
				receiveJobMessage();
			}

			if (isMaster) {
				if (status.MPI_TAG == CMD_END) {
					receiveEnd(status);
				}
			} else {
				if (status.MPI_TAG == CMD_SENDJOB) {
					receiveSendJob(status);
				}
			}
		}

		first = false;
	} while (flag);
}

void Process::sendMessages()
{
	if (sendBest) {
		sendBestToAll();

		if (myBestPrice == bestPricePossible) {
			for (int i = 0; i < processCount; ++i) {
				if (i != processNum) {
					sendStop(i);
				}				
			}

			stop();
			return;
		}

		sendBest = false;
	}

	if (sendJobTo != -1) {
		divideJobAndSend();
	}

	if (!isMaster) {
		// send END
		if (state == STATE_WORKING && !iter->hasNext()) {
			sendEnd();
			setState(STATE_NOT_WORKING);
		}
	}

	if (isMaster) {
		vector<int> waitingForJob;
		vector<int> working;

		for (int i = 0; i < processCount; ++i) { // do not check master
			if (processes[i] == STATE_WORKING) {
				if (!isMaster || iter->hasNext()) {
					working.push_back(i);
				}
			}

			if (processes[i] == STATE_NOT_WORKING) {
				waitingForJob.push_back(i);
			}
		}

		/*
		cout << "waiting:";
		for (unsigned int i = 0; i < waitingForJob.size(); ++i)
		{
			cout << " " << waitingForJob.at(i);
		}
		cout << endl;

		cout << "working:";
		for (unsigned int i = 0; i < working.size(); ++i)
		{
			cout << " " << working.at(i);
		}
		cout << endl;
		*/

		// send sendjob
		for (unsigned int i = 0; i < min(waitingForJob.size(), working.size()); ++i) {
			if (working.at(i) == 0) {
				sendJobTo = waitingForJob.at(i);
				divideJobAndSend();
			} else {
				sendSendJob(working.at(i), waitingForJob.at(i));
			}
			processes[waitingForJob.at(i)] = STATE_WAITING;
		}

		if (working.size() == 0) {
			for (int i = 1; i < processCount; i++) { // do not send stop to master
				sendStop(i);
			}

			stop();
		}
	}
}

void Process::receiveBest(MPI_Status & status)
{
	cout << "[P" << processNum << "] received new best" << endl;
	char * buf = new char[combinationSize];
	MPI_Recv(buf, combinationSize, MPI_CHAR, MPI_ANY_SOURCE, CMD_NEWBEST, MPI_COMM_WORLD, &status);
	if (checkSolution(deserializeCombination(buf))) {
		sendBest = false;
	}
}

void Process::divideJobAndSend()
{
	CombinationIterator * half = iter->divide();
	sendJobMessage(sendJobTo, half);
	delete half;
	cout << "[P" << processNum << "] Job sent from " << processNum << " to " << sendJobTo << endl;
	sendJobTo = -1;
}

void Process::receiveStop(MPI_Status & status)
{
	char * buf = new char[0];
	MPI_Recv(buf, 0, MPI_CHAR, MPI_ANY_SOURCE, CMD_STOP, MPI_COMM_WORLD, &status);
	cout << "[P" << processNum << "] received STOP" << endl;
	stopped = true;
	delete [] buf;
}

void Process::receiveEnd(MPI_Status & status)
{
	char * buf = new char[0];
	MPI_Recv(buf, 0, MPI_CHAR, MPI_ANY_SOURCE, CMD_END, MPI_COMM_WORLD, &status);
	processes[status.MPI_SOURCE] = STATE_NOT_WORKING;
	cout << "[P" << processNum << "] received END from " << status.MPI_SOURCE << endl;
	delete [] buf;
}

void Process::receiveSendJob(MPI_Status & status)
{
	int * buf = new int[1];
	MPI_Recv(buf, 1, MPI_INT, MPI_ANY_SOURCE, CMD_SENDJOB, MPI_COMM_WORLD, &status);
	cout << "[P" << processNum << "] received SENDJOB(" << buf[0] << ") from " << status.MPI_SOURCE << endl;
	sendJobTo = buf[0];
	delete [] buf;
}

void Process::sendSendJob(int from, int to)
{
	int * buf = new int[1];
	buf[0] = to;
	MPI_Send(buf, 1, MPI_INT, from, CMD_SENDJOB, MPI_COMM_WORLD);
	cout << "[P" << processNum << "] sending SENDJOB from " << from << " to " << to << endl;
	delete [] buf;
}

void Process::receiveJobMessage()
{
	cout << "[P" << processNum << "] receiving job message" << endl;
	MPI_Status * status = 0;
	int msgSize = combinationSize * 2;
	char * msg = new char[msgSize];

	MPI_Recv(msg, msgSize, MPI_CHAR, 0, Process::CMD_JOB, MPI_COMM_WORLD, status);

	this->iter = deserializeIterator(msg);

	cout << "[P" << processNum << "] receive job from ";
	iter->getCurrent()->print();
	cout << " to ";
	iter->getMax()->print();
	cout << endl;

	delete [] msg;

	setState(STATE_WORKING);
}

void Process::sendJobMessage(int proc, CombinationIterator * interval) const
{
	cout << "[P" << processNum << "] sending job from ";
	interval->getCurrent()->print();
	cout << " to ";
	interval->getMax()->print();
	cout << endl;

	char * msg = serializeIterator(interval);
	MPI_Send(msg, interval->getCurrent()->getSize() * 2, MPI_CHAR, proc, CMD_JOB, MPI_COMM_WORLD);
	delete [] msg;
}

void Process::sendStop(int proc)
{
	cout << "[P" << processNum << "] sending STOP to " << proc << endl;
	char * msg = new char[0];
	MPI_Send(msg, 0, MPI_CHAR, proc, CMD_STOP, MPI_COMM_WORLD);
	delete [] msg;

	if (isMaster) {
		processes[proc] = STATE_FINISHED;
	}
}

void Process::sendEnd()
{
	cout << "[P" << processNum << "] sending END" << endl;

	char * msg = new char[0];
	MPI_Send(msg, 0, MPI_CHAR, 0, CMD_END, MPI_COMM_WORLD);
	delete [] msg;
}

void Process::sendBestToAll()
{
	char * msg = serializeCombination(myBestSolution->getCombination());
	for (int i = 0; i < processCount; ++i) {
		if (i != processNum) {
			cout << "[P" << processNum << "] sending new best ";
			myBestSolution->getCombination()->print();
			cout << " (price " << myBestSolution->getPrice() << ")" << endl;

			MPI_Send(msg, combinationSize, MPI_CHAR, i, CMD_NEWBEST, MPI_COMM_WORLD);
		}
	}
	delete [] msg;
}

void Process::stop()
{
	stopped = true;
	cout << "[P" << processNum << "] stopping" << endl;
}

//////////////////////////////////////////////////////////////////
// SERIALIZATION
//////////////////////////////////////////////////////////////////

char * Process::serializeCombination(EdgeCombination * c) const
{
	int combSize = c->getSize();
	char * msg = new char[combSize];

	for (int i = 0; i < combSize; i++) {
		msg[i] = c->isRed(i) ? 'r' : 'y';
	}

	return msg;
}

EdgeCombination * Process::deserializeCombination(char * buf) const
{
	EdgeCombination * c = new EdgeCombination(combinationSize);

	for (int i = 0; i < combinationSize; ++i) {
		c->setColor(i, buf[i] == 'r' ? RED : YELLOW);
	}

	return c;
}

char * Process::serializeIterator(CombinationIterator * interval) const
{
	EdgeCombination * start = interval->getCurrent();
	EdgeCombination * end = interval->getMax();

	int combSize = start->getSize();
	int msgSize = combSize * 2;
	char * msg = new char[msgSize];

	for (int i = 0; i < combSize; i++) {
		msg[i] = start->isRed(i) ? 'r' : 'y';
	}

	for (int i = 0; i < combSize; i++) {
		msg[combSize + i] = end->isRed(i) ? 'r' : 'y';
	}

	return msg;
}

CombinationIterator * Process::deserializeIterator(char * buf) const
{
	return new CombinationIterator(
		deserializeCombination(buf),
		deserializeCombination(&buf[combinationSize])
	);
}

void Process::setState(ProcessState s)
{
	state = s;
	if (isMaster) {
		processes[0] = s;
	}
}