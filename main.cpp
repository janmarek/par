#include <iostream>
#include "Graph.h"
#include "GraphFactory.h"

using namespace std;

// main
int main(int argc, char *argv[])
{
	GraphFactory factory;
	
	Graph * g = factory.createFromStream(std::cin);
	
	cout << "nodes " << g->getNodesCount() << endl;
	
	delete g;
	return 0;
}
