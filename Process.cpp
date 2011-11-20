#include <iostream>
#include <vector>
#include "CombinationIterator.h"
#include "Graph.h"
#include "Solution.h"
#include "Process.h"

using namespace std;

Process::Process(Graph * graph, bool isMaster, int processCount)
{
	this->graph = graph;
	this->isMaster = isMaster;
	this->processCount = processCount;
	iter = 0;
	stopped = false;
	bestSolutionPossible = graph->getEdgeCount() % 2;
	myBestPrice = -1;
}

void Process::run()
{
	int counter = 0;
	
	if (isMaster) {
		// TODO init
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
			checkMessages();		
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
		delete myBestSolution;
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
