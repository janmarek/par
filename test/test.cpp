#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "asserts.cpp"
#include "../Graph.h"
#include "../GraphFactory.h"
#include "../Solution.h"
#include "../EdgeCombination.h"
#include "../Color.cpp"
#include "../CombinationIterator.h"

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

void testTestTriangle()
{
	cout << "Test triangle" << endl;
	
	Graph * g = new Graph(4);
	
	assertFalse(g->testTriangle(0, 1, 3));
	
	g->addEdge(0, 1);
	g->addEdge(0, 2);
	g->addEdge(2, 1);
	
	assertTrue(g->testTriangle(0, 1, 2));

	delete g;
	
	cout << endl;
}

void testGetNeighbours()
{
	cout << "Test get neighbours" << endl;
	
	Graph * g = new Graph(4);
	
	vector<int> vc = g->getNeighbours(0);
	
	assert(vc.size() == 0);
	
	g->addEdge(0, 1);
	g->addEdge(0, 2);
	g->addEdge(2, 1);
	
	vector<int> vc2 = g->getNeighbours(0);
	
	assert(vc2.size() == 2);
	assert(vc2[0] == 1);
	assert(vc2[1] == 2);

	delete g;
	
	cout << endl;
}

void testCombination()
{
	cout << "Test combination" << endl;
	
	Color arr [] = {YELLOW, YELLOW, RED};
	
	EdgeCombination * combination = new EdgeCombination(3, arr);
	
	assert(combination->isRed(0));
	assert(combination->getColor(1) == YELLOW);
	assert(combination->isYellow(2));
	
	EdgeCombination * combination2 = new EdgeCombination(2);
	
	assert(combination2->isRed(0));
	assert(combination2->isRed(1));
	
	delete combination;
	delete combination2;
	
	cout << endl;
}

void testCombinationIncrement()
{
	cout << "Test combination increment" << endl;
	
	EdgeCombination * combination = new EdgeCombination(3);
	
	combination->increment();
	assert(combination->isYellow(0));
	assert(combination->isRed(1));
	assert(combination->isRed(2));
	
	combination->increment();
	assert(combination->isRed(0));
	assert(combination->isYellow(1));
	assert(combination->isRed(2));
	
	combination->increment();
	assert(combination->isYellow(0));
	assert(combination->isYellow(1));
	assert(combination->isRed(2));
	
	combination->increment();
	assert(combination->isRed(0));
	assert(combination->isRed(1));
	assert(combination->isYellow(2));
	
	combination->increment();
	assert(combination->isYellow(0));
	assert(combination->isRed(1));
	assert(combination->isYellow(2));
	
	combination->increment();
	assert(combination->isRed(0));
	assert(combination->isYellow(1));
	assert(combination->isYellow(2));
	
	combination->increment();
	assert(combination->isYellow(0));
	assert(combination->isYellow(1));
	assert(combination->isYellow(2));
	
	combination->increment();
	assert(combination->isRed(0));
	assert(combination->isRed(1));
	assert(combination->isRed(2));
	
	delete combination;
	
	cout << endl;
}

void testCombinationOperators()
{
	cout << "Test combination operators" << endl;
	
	EdgeCombination * c = new EdgeCombination(2);
	
	Color arr [] = {RED, RED};
	
	EdgeCombination * c2 = new EdgeCombination(2, arr);
	
	assert(c->equals(c2));
	c2->increment();
	assertFalse(c->equals(c2));
	c2->increment();
	assertFalse(c->equals(c2));
	c2->increment();
	assertFalse(c->equals(c2));
	c2->increment();
	assert(c->equals(c2));
	
	delete c;
	delete c2;
	
	cout << endl;
}

void testCombinationCounts()
{
	cout << "Test combination counts" << endl;
	
	Color arr [] = {RED, RED, YELLOW, RED, YELLOW};
	
	EdgeCombination * c = new EdgeCombination(5, arr);
	
	assert(c->getYellowCount() == 2);
	assert(c->getRedCount() == 3);
	
	delete c;
	
	cout << endl;
}

void testCombinationClone()
{
	cout << "Test combination clone" << endl;
	
	Color arr [] = {RED, RED, YELLOW, RED, YELLOW};
	
	EdgeCombination * c = new EdgeCombination(5, arr);
	EdgeCombination * cloned = c->clone();
	
	assert(c->equals(cloned));
	assert(c != cloned);
	
	delete c;
	delete cloned;
	
	cout << endl;
}

void testIterator()
{
	cout << "Test iterator" << endl;
	
	Color arrMax [] = {RED, YELLOW, YELLOW, YELLOW};
	Color arrOne [] = {RED, RED, RED, YELLOW};
	
	EdgeCombination * c = new EdgeCombination(4);
	EdgeCombination * one = new EdgeCombination(4, arrOne);
	EdgeCombination * cMax = new EdgeCombination(4, arrMax);
	
	CombinationIterator * it = new CombinationIterator(c, cMax);
	
	EdgeCombination * next = it->next();
	assert(next->equals(it->getCurrent()));
	assert(next->equals(one));
	
	int counter = 0;
	
	while (it->hasNext()) {
		it->next();
		counter++;
	}

	assert(counter == 6);
	
	delete it;
	delete one;
	
	cout << endl;
}

int main (int argc, char const* argv[])
{
	cout << "Graph tests" << endl << endl;
	testAddAndHasEdge();
	testRemoveEdge();
	testEdgeCount();
	testTestTriangle();
	testGetNeighbours();
	
	cout << endl << "Factory tests" << endl << endl;
	testFactory();
	
	cout << endl << "Combination tests" << endl << endl;
	testCombination();
	testCombinationIncrement();
	testCombinationOperators();
	testCombinationCounts();
	testCombinationClone();
	
	cout << endl << "Iterator tests" << endl << endl;
	testIterator();
	
	return 0;
}
