#include <iostream>
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
	state = STATE_NEW;
	sendBest = false;

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

		vector<ProcessState> prVc(processCount, STATE_WORKING);
		this->processes = prVc;

		for (unsigned int i = 1; i < intervals.size(); ++i) {
			sendJobMessage(i, intervals[i]);
		}

		cout << "[P0] initial jobs sent" << endl;
		state = STATE_WORKING;
	}
	
	while (true) {
		if (stopped) {
			break;
		}
	
		// iter nemusi existovat u procesu, co jeste nedostal JOB
		if (iter != 0) {
			// zkontroluje aktualni solution a pripadne aktualizuje promenne
			checkSolution(iter->getCurrent()->clone());
	
			// prepni na dalsi
			if (iter->hasNext()) {
				iter->next();
			}
			
			counter++;
			
			// message check and send
			if ((isMaster && counter == MASTER_TIMEOUT) || counter == TIMEOUT || !iter->hasNext() || myBestPrice == bestPricePossible) {
				counter = 0;
				checkMessages();
				sendMessages();
			}
		} else {
			receiveJobMessage();
		}
	}
	
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

void Process::checkSolution(EdgeCombination * c)
{
	// create solution: graph is shared instance, combination instance is cloned
	Solution * s = new Solution(graph, c);

	// triangle test and price test
	if (graph->testTriangleOk(iter->getCurrent()) && (myBestPrice == -1 || myBestPrice > s->getPrice())) {
		// update data
		myBestPrice = s->getPrice();

		cout << "[P" << processNum << "] update best with ";
		s->getCombination()->print();
		cout << " (price " << myBestPrice << ")" << endl;
		sendBest = true;

		myBestSolution = s;
	} else {
		// delete current solution instance
		delete s;
	}
}


void Process::checkMessages()
{
	//cout << "[P" << processNum << "] processing messages" << endl;

	MPI_Status status;
	int flag = 0;

	MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

	if (status.MPI_TAG == CMD_NEWBEST) {
		cout << "[P" << processNum << "] received new best" << endl;
		char * buf = new char[combinationSize];
		MPI_Recv(buf, combinationSize, MPI_CHAR, MPI_ANY_SOURCE, CMD_NEWBEST, MPI_COMM_WORLD, &status);
		checkSolution(deserializeCombination(buf));
		sendBest = false;
	}

	if (status.MPI_TAG == CMD_STOP) {
		char * buf = new char[0];
		MPI_Recv(buf, 0, MPI_CHAR, MPI_ANY_SOURCE, CMD_STOP, MPI_COMM_WORLD, &status);
		cout << "[P" << processNum << "] received STOP" << endl;
		stopped = true;
		delete [] buf;
	}

	if (isMaster) {
		if (status.MPI_TAG == CMD_END) {
			char * buf = new char[0];
			MPI_Recv(buf, 0, MPI_CHAR, MPI_ANY_SOURCE, CMD_END, MPI_COMM_WORLD, &status);
			processes[status.MPI_SOURCE] = STATE_NOT_WORKING;
			cout << "[P" << processNum << "] received END from " << status.MPI_SOURCE << endl;
			delete [] buf;
		}
	} else {
		if (status.MPI_TAG == CMD_JOB) {
			receiveJobMessage();
		}
	}

	// MPI_Iprobe ( int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status ) 
	
	// TODO
	// zkontroluj NEWBEST a pripadne se ukonci (v pripade nalezeni reseni s cenou bestPricePossible)
	// pokud jsi slave, zkontroluj SENDJOB
	// pokud jsi slave, cekej na JOB nebo na STOP (blokujici receive)
}

void Process::sendMessages()
{
	//cout << "[P" << processNum << "] sending messages" << endl;
	
	if (sendBest) {
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

		if (myBestPrice == bestPricePossible) {
			char * msg = new char[0];

			for (int i = 0; i < processCount; ++i) {
				if (i != processNum) {
					cout << "[P" << processNum << "] sending STOP to " << i << endl;
					MPI_Send(msg, 0, MPI_CHAR, i, CMD_STOP, MPI_COMM_WORLD);
				}				
			}
			delete [] msg;

			cout << "[P" << processNum << "] stopping" << endl;
			stopped = true;
			return;
		}

		sendBest = false;
	}

	if (isMaster) {
		bool allFinished = true;

		for (int i = 1; i < processCount; ++i) { // do not check master
			if (processes[i] == STATE_NOT_WORKING) {
				cout << "[P" << processNum << "] sending STOP to " << i << endl;
				char * msg = new char[0];
				MPI_Send(msg, 0, MPI_CHAR, i, CMD_STOP, MPI_COMM_WORLD);
				delete [] msg;
				processes[i] = STATE_FINISHED;
			}

			if (processes[i] != STATE_FINISHED) {
				allFinished = false;
			}
		}

		if (allFinished && !iter->hasNext()) {
			stopped = true;
			cout << "[P" << processNum << "] stopping" << endl;
		}
	}

	if (!isMaster) {
		// send END
		if (state == STATE_WORKING && !iter->hasNext()) {
			cout << "[P" << processNum << "] sending END" << endl;

			char * msg = new char[0];
			MPI_Send(msg, 0, MPI_CHAR, 0, CMD_END, MPI_COMM_WORLD);
			state = STATE_NOT_WORKING;
			delete [] msg;
		}
	}

	// zkontroluj, jestli nemuzes poslat NEWBEST (pokud je bestPricePossible, tak uz nic dalsiho neposilej)
	// pokud jsi slave a nemas praci, posli END
	// pokud jsi master a nekdo chtel praci, posli nekomu SENDJOB
	// pokud jsi master a vse je hotovo, posli prikazy STOP vsem slaveum	
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

	state = STATE_WORKING;
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
