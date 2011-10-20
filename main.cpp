#include <iostream>
#include "Graph.h"
#include "GraphFactory.h"
#include "GraphWalker.h"

using namespace std;

// main
int main(int argc, char *argv[])
{
	GraphFactory factory;
	
	Graph * g = factory.createFromStream(std::cin);
	
	GraphWalker walker;
	
	cout << "nodes " << g->getNodesCount() << endl;
	
	Result * res = walker.process(g);
	
	// v tom resultu je zatim jen nejaka blbost, tak ho ani nevypisuju
	
	delete res;
	delete g;
	return 0;
}
