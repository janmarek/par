#include <iostream>
#include "Graph.h"
#include "GraphFactory.h"
#include "Solution.h"
#include "EdgeCombination.h"
#include "CombinationIterator.h"
#include <fstream>
#include <mpi/mpi.h>

using namespace std;

int myBestPrice;
Solution * myBestSolution;
int bestSolutionPossible;
Graph * g;

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
        
        if (myRank== 0) // master
        {
            MPI_Barrier(MPI_COMM_WORLD); // nevim, jestli to tu musi byt
            //todo
        }        
        else // slave
        {
            MPI_Barrier(MPI_COMM_WORLD); // nevim, jestli to tu musi byt
            GraphFactory factory;

            //create graph
            ifstream ifs("fixtures/graf6.txt", ifstream::in);
            g = factory.createFromStream(ifs); //
            //Graph * g = factory.createFromStream(std::cin);

            int edgeCount = g->getEdgeCount();
            cout << "nodes: " << g->getNodesCount() << ", edges: " <<  edgeCount << endl;

            // best price
            myBestPrice = -1;
            // best solution
            myBestSolution = 0;
            // nejlepsi dosazitelne reseni je 0 pro sudy pocet hran a 1 pro lichy
            bestSolutionPossible = edgeCount % 2;

            // napr.: {RED, RED, RED}
            //EdgeCombination * c = new EdgeCombination(edgeCount); // spolecny zaklad pro vsechny procesy
            // napr.: {YELLOW, YELLOW, YELLOW}        
            //EdgeCombination * max = EdgeCombination::createMaxCombination(edgeCount);	 
            
            // prace, ktera bude pridelena jednomu procesu. pocet hran / pocet dostupnych procesu
            int oneProcessWork = edgeCount / (p-1);
            int edgeStart = 0;
            int edgeEnd = -1;            
            for (int i = 1; i < p ; i++) // rozdeli praci vsem procesum / hlavni proces 0 je vynechan
            {
                if (i == myRank) // zpracovava pouze proces cislo myRank / hlavni proces 0 neresi vypocty
                {
                    edgeStart = edgeEnd + 1;                    
                    edgeEnd =  edgeStart + oneProcessWork;                    
                    EdgeCombination * c = new EdgeCombination(edgeCount, edgeStart);
                    EdgeCombination * max = new EdgeCombination(edgeCount, edgeEnd);        
                    doSearch(c, max);
                }
            }                        
        }                                                   	
	// print solution
        if (myRank == 0)
        {
            if (myBestPrice != -1) {
                    myBestSolution->print();
                    delete myBestSolution;
            }
        }
		
	delete g;
	
        // Finalizace MPI - zadma MPI funkce nesmi byt po teto funkci
        MPI_Finalize();
	return 0;
}

void doSearch(EdgeCombination c, EdgeCombination max)
{
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
        delete it;
}
