#include <iostream>
#include "Graph.h"
#include "GraphFactory.h"
#include "Solution.h"
#include "EdgeCombination.h"
#include "CombinationIterator.h"
#include <fstream>
#include <mpi/mpi.h>

using namespace std;

// main
// nelze nacitat grafy pres <, nutno pouzit paramater
int main(int argc, char **argv)
{
        int myRank; // cislo procesu
        int p; // pocet procesu
        // Inicializace MPI knihovny: http://www.mcs.anl.gov/research/projects/mpi/www/www3/MPI_Init.html
        MPI_Init(&argc, &argv); // zadna MPI funkce nesmi byt pred touto funkci
        // Zjisti cislo procesu. 0 je hlavni proces, ostatni maji cisla ruzna od nuly. Vysledek ulozi do myRank
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
        // Ulozi do p pocet dostupnych procesu
        MPI_Comm_size(MPI_COMM_WORLD, &p);
        
//--------///Tato cast je pro vsechny procesy vypocetne spolecna//---------///        
	GraphFactory factory;
	
	//create graph
        ifstream ifs("fixtures/graf6.txt", ifstream::in);
        Graph * g = factory.createFromStream(ifs); //
        //Graph * g = factory.createFromStream(std::cin);

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
		
        
//----------------------------------------------------------------------------//                
        //nejjednodussi by bylo, kdyby kazdy proces mel svuj iterator s adekvatnimi hranicemi, ale to bychom pak nic dynamicky na zadost procesu nerozdelovali
        // 
        
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
        if (myRank == 0)
        {
            if (myBestPrice != -1) {
                    myBestSolution->print();
                    delete myBestSolution;
            }
        }
	
	delete it;
	delete g;
	
        // Finalizace MPI - zadma MPI funkce nesmi byt po teto funkci
        MPI_Finalize();
	return 0;
}
