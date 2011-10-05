#include <iostream>
#include "Graph.h"

using namespace std;

// tests
void assert(bool condition)
{
	cout << (condition ? '.' : 'F');
}

void assertTrue(bool condition)
{
	assert(condition == true);
}

void assertFalse(bool condition)
{
	assert(condition == false);
}

void testAddAndHasEdge()
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

void testRemoveEdge()
{
	Graph * g = new Graph(6);

	g->addEdge(0, 2);

	assertTrue(g->hasEdge(0, 2));

	g->removeEdge(0, 2);
	assertFalse(g->hasEdge(0, 2));

	delete g;
}


// main
int main(int argc, char * argv[])
{
	testAddAndHasEdge();
	testRemoveEdge();
	cout << endl;
	return 0;
}
