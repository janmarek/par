#include <iostream>
#include "Graph.h"
#include "GraphFactory.h"
#include "Solution.h"
#include "EdgeCombination.h"
#include "CombinationIterator.h"

using namespace std;

// main
int main(int argc, char *argv[])
{
	GraphFactory factory;
	
	// create graph
	Graph * g = factory.createFromStream(std::cin);

	int edgeCount = g->getEdgeCount();
	cout << "nodes: " << g->getNodesCount() << ", edges: " <<  edgeCount << endl;
	
	// best price
	int myBestPrice = -1;
	
	// best solution
	Solution * myBestSolution = 0;
	
	// nejlepsi dosazitelne reseni je 0 pro sudy pocet hran a 1 pro lichy
	int bestSolutionPossible = edgeCount % 2;
	
	// napr.: {RED, RED, RED}
	EdgeCombination * c = new EdgeCombination(edgeCount);
	
	// napr.: {YELLOW, YELLOW, YELLOW}
	EdgeCombination * max = EdgeCombination::createMaxCombination(edgeCount);
	
	// iterator from c to max
	CombinationIterator * it = new CombinationIterator(c, max);
	
	do {
		// create solution: graph is shared instance, combination instance is cloned
		Solution * s = new Solution(g, it->getCurrent()->clone());
	
		// triangle test and price test
		if (g->testTriangleOk(it->getCurrent()) && (myBestPrice == -1 || myBestPrice > s->getPrice())) {
			// update data
			myBestPrice = s->getPrice();
			delete myBestSolution;
			myBestSolution = s;
			
			// if price is best possible then quit algorithm
			if (myBestPrice == bestSolutionPossible) {
				break;
			}
		} else {
			// delete current solution instance
			delete s;
		}
		
		// prepni na dalsi
		it->next();
	} while (it->hasNext());
	
	// print solution
	if (myBestPrice != -1) {
		myBestSolution->print();
		delete myBestSolution;
	}
	
	delete it;
	delete g;
	
	return 0;
}
