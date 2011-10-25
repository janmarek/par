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
	
	cout << "nodes: " << g->getNodesCount() << ", edges: " <<  g->getEdgeCount() << endl;
	        
	Solution sol = walker.process(g);
	
	
	//delete sol;
	delete g;
	return 0;
}
