#include "asserts.cpp"
#include "Graph.h"
#include "GraphTest.h"

void GraphTest::testAddAndHasEdge()
{
	Graph * g = new Graph(3);

	assertFalse(g->hasEdge(0, 1));
	assertFalse(g->hasEdge(0, 2));
	assertFalse(g->hasEdge(2, 1));

	g->addEdge(0, 2);

	assertFalse(g->hasEdge(0, 1));
	assertTrue(g->hasEdge(0, 2));
	assertTrue(g->hasEdge(2, 0));

	delete g;
}

void GraphTest::testRemoveEdge()
{
	Graph * g = new Graph(6);

	g->addEdge(0, 2);

	assertTrue(g->hasEdge(0, 2));

	g->removeEdge(0, 2);
	assertFalse(g->hasEdge(0, 2));

	delete g;
}

void GraphTest::testEdgeCount()
{
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
}