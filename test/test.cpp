#include <iostream>
#include <string>
#include <sstream>
#include "asserts.cpp"
#include "../Graph.h"
#include "../GraphFactory.h"

using namespace std;

void testAddAndHasEdge()
{
	cout << "Test add and has edge" << endl;
	
	Graph * g = new Graph(3);

	assertFalse(g->hasEdge(0, 1));
	assertFalse(g->hasEdge(0, 2));
	assertFalse(g->hasEdge(2, 1));

	g->addEdge(0, 2);

	assertFalse(g->hasEdge(0, 1));
	assertTrue(g->hasEdge(0, 2));
	assertTrue(g->hasEdge(2, 0));

	delete g;
	
	cout << endl;
}

void testRemoveEdge()
{
	cout << "Test remove edge" << endl;

	Graph * g = new Graph(6);

	g->addEdge(0, 2);

	assertTrue(g->hasEdge(0, 2));

	g->removeEdge(0, 2);
	assertFalse(g->hasEdge(0, 2));

	delete g;
	
	cout << endl;
}

void testEdgeCount()
{
	cout << "Test edge count" << endl;

	Graph * g = new Graph(3);
	
	assert(g->getEdgeCount() == 0);

	g->addEdge(0, 1);
	g->addEdge(1, 0);
	
	assert(g->getEdgeCount() == 1);
	
	g->addEdge(0, 2);
	
	assert(g->getEdgeCount() == 2);
	
	g->addEdge(2, 1);
	
	assert(g->getEdgeCount() == 3);
	
	g->removeEdge(2, 0);
	
	assert(g->getEdgeCount() == 2);
	
	cout << endl;
}

void testFactory()
{
	cout << "Test factory creates graph from stream" << endl;

	string s = "3\n010\n101\n010";
	istringstream stream(s, istringstream::in);
	GraphFactory factory;
	
	Graph * g = factory.createFromStream(stream);
	assert(g->getEdgeCount() == 2);	
	delete g;
	
	cout << endl;
}

int main (int argc, char const* argv[])
{
	testAddAndHasEdge();
	testRemoveEdge();
	testEdgeCount();
	testFactory();
	return 0;
}
