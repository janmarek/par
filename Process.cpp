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
	bestSolutionPossible = graph->getEdgeCount() % 2;
	myBestPrice = -1;
}

void Process::run()
{
	int counter = 0;
	
	if (isMaster) {
		cout << "[P0] initializing master" << endl;

		int ec = graph->getEdgeCount();
		CombinationIterator * tmp = new CombinationIterator(new EdgeCombination(ec), EdgeCombination::createMaxCombination(ec));
		vector<CombinationIterator *> intervals = tmp->divide(processCount);
		delete tmp;

		this->iter = intervals[0];

		for (unsigned int i = 1; i < intervals.size(); ++i) {
			sendJobMessage(i, intervals[i]);
		}

		cout << "[P0] initial jobs sent" << endl;
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
			iter->next();
			
			counter++;
			
			// message check and send
			if ((isMaster && counter == MASTER_TIMEOUT) || counter == TIMEOUT) {
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

		myBestSolution = s;
	} else {
		// delete current solution instance
		delete s;
	}
}


void Process::checkMessages()
{

	// TODO
	// zkontroluj NEWBEST a pripadne se ukonci (v pripade nalezeni reseni s cenou bestPricePossible)
	// pokud jsi slave, zkontroluj SENDJOB
	// pokud jsi slave, cekej na JOB nebo na STOP (blokujici receive)
}

void Process::sendMessages() const
{
	// TODO
	// zkontroluj, jestli nemuzes poslat NEWBEST (pokud je bestPricePossible, tak uz nic dalsiho neposilej)
	// pokud jsi slave, posli END nebo STATE (podle toho, jestli mas praci)
	// pokud jsi master a nekdo chtel praci, posli nekomu SENDJOB
	// pokud jsi master a vse je hotovo, posli prikazy STOP vsem slaveum	
}

void Process::receiveJobMessage()
{
	MPI_Status * status = 0;
	int combSize = graph->getEdgeCount();
	int msgSize = combSize * 2;
	char * msg = new char[msgSize];

	MPI_Recv(msg, msgSize, MPI_CHAR, 0, Process::CMD_JOB, MPI_COMM_WORLD, status);

	if (iter != 0) {
		delete iter;
	}

	EdgeCombination * start = new EdgeCombination(combSize);

	for (int i = 0; i < combSize; ++i) {
		start->setColor(i, msg[i] == 'r' ? RED : YELLOW);
	}

	EdgeCombination * end = new EdgeCombination(combSize);

	for (int i = 0; i < combSize; ++i) {
		end->setColor(i, msg[combSize + i] == 'r' ? RED : YELLOW);
	}

	this->iter = new CombinationIterator(start, end);

	cout << "[P" << processNum << "] receive job from ";
	start->print();
	cout << " to ";
	end->print();
	cout << endl;

	delete [] msg;
}

void Process::sendJobMessage(int processNum, CombinationIterator * interval) const
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

	MPI_Send(msg, msgSize, MPI_CHAR, processNum, Process::CMD_JOB, MPI_COMM_WORLD);

	delete [] msg;
}
