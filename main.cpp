#include <iostream>
#include <fstream>
#include <mpi/mpi.h>
#include "Graph.h"
#include "GraphFactory.h"
#include "Process.h"

using namespace std;

// main
int main(int argc, char *argv[])
{
	int currentProcess; // cislo procesu
	int processCount; // pocet procesu
	// Inicializace MPI knihovny: http://www.mcs.anl.gov/research/projects/mpi/www/www3/MPI_Init.html
	MPI_Init(&argc, &argv);
	// Zjisti cislo procesu. 0 je hlavni proces, ostatni maji cisla ruzna od nuly
	MPI_Comm_rank(MPI_COMM_WORLD, &currentProcess);
	// Ulozi do p pocet dostupnych procesu
	MPI_Comm_size(MPI_COMM_WORLD, &processCount);
	
	cout << "Running process " << currentProcess << " of " << processCount << "." << endl;

	// create graph (pokud se zada v prvnim parametru nazev souboru, mel by se nacist ten)
	GraphFactory factory;
	
	Graph * graph = 0;
	
	if (argc > 0) {
		ifstream fstream(argv[0]);
		graph = factory.createFromStream(fstream);
	} else {
		graph = factory.createFromStream(cin);
	}
	
	Process * process = new Process(graph, currentProcess, processCount);
	process->run();
	
	delete process;
	delete graph;
	
	MPI_Finalize();
	
	return 0;
}
