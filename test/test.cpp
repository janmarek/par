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

void testCombinationAdd()
{
	cout << "Test combination add" << endl;
	
	EdgeCombination * zero = new EdgeCombination(3);
	EdgeCombination * one = new EdgeCombination(3);
	one->setColor(0, YELLOW);
	EdgeCombination * two = new EdgeCombination(3);
	two->setColor(1, YELLOW);
	EdgeCombination * three = new EdgeCombination(3);
	three->setColor(0, YELLOW);
	three->setColor(1, YELLOW);
	EdgeCombination * four = new EdgeCombination(3);
	four->setColor(2, YELLOW);
	EdgeCombination * five = new EdgeCombination(3);
	five->setColor(0, YELLOW);
	five->setColor(2, YELLOW);
	EdgeCombination * six = new EdgeCombination(3);
	six->setColor(1, YELLOW);
	six->setColor(2, YELLOW);
	EdgeCombination * seven = new EdgeCombination(3);
	seven->setColor(0, YELLOW);
	seven->setColor(1, YELLOW);
	seven->setColor(2, YELLOW);
	
	EdgeCombination * test = new EdgeCombination(3);
	assert(test->add(zero)->equals(zero));
	assert(test->add(one)->equals(one));
	assert(test->add(one)->equals(two));
	assert(test->add(three)->equals(five));
	assert(test->add(two)->equals(seven));
	
	EdgeCombination * test2 = three->clone();
	assert(test2->add(three)->equals(six));
	assert(test2->add(one)->equals(seven));
	
	delete zero;
	delete one;
	delete two;
	delete three;
	delete four;
	delete five;
	delete six;
	delete seven;
	delete test2;
	delete test;
	
	cout << endl;
}

void testCombinationDivideBy2()
{
	cout << "Test combination divide by 2" << endl;
	
	EdgeCombination * zero = new EdgeCombination(3);
	EdgeCombination * one = new EdgeCombination(3);
	one->setColor(0, YELLOW);
	EdgeCombination * two = new EdgeCombination(3);
	two->setColor(1, YELLOW);
	EdgeCombination * three = new EdgeCombination(3);
	three->setColor(0, YELLOW);
	three->setColor(1, YELLOW);
	EdgeCombination * four = new EdgeCombination(3);
	four->setColor(2, YELLOW);
	EdgeCombination * five = new EdgeCombination(3);
	five->setColor(0, YELLOW);
	five->setColor(2, YELLOW);
	EdgeCombination * six = new EdgeCombination(3);
	six->setColor(1, YELLOW);
	six->setColor(2, YELLOW);
	EdgeCombination * seven = new EdgeCombination(3);
	seven->setColor(0, YELLOW);
	seven->setColor(1, YELLOW);
	seven->setColor(2, YELLOW);
	
	assert(six->divideBy2()->equals(three));
	assert(seven->divideBy2()->equals(three));
	assert(one->divideBy2()->equals(zero));
	assert(five->divideBy2()->equals(two));
	
	delete zero;
	delete one;
	delete two;
	delete three;
	delete four;
	delete five;
	delete six;
	delete seven;
	
	cout << endl;
}

void testCombinationSubtract()
{
	cout << "Test combination subtract" << endl;
	
	EdgeCombination * zero = new EdgeCombination(3);
	EdgeCombination * one = new EdgeCombination(3);
	one->setColor(0, YELLOW);
	EdgeCombination * two = new EdgeCombination(3);
	two->setColor(1, YELLOW);
	EdgeCombination * three = new EdgeCombination(3);
	three->setColor(0, YELLOW);
	three->setColor(1, YELLOW);
	EdgeCombination * four = new EdgeCombination(3);
	four->setColor(2, YELLOW);
	EdgeCombination * five = new EdgeCombination(3);
	five->setColor(0, YELLOW);
	five->setColor(2, YELLOW);
	EdgeCombination * six = new EdgeCombination(3);
	six->setColor(1, YELLOW);
	six->setColor(2, YELLOW);
	EdgeCombination * seven = new EdgeCombination(3);
	seven->setColor(0, YELLOW);
	seven->setColor(1, YELLOW);
	seven->setColor(2, YELLOW);
	
	EdgeCombination * test = seven->clone();
	assert(test->subtract(zero)->equals(seven));
	assert(test->subtract(one)->equals(six));
	assert(test->subtract(one)->equals(five));
	assert(test->subtract(three)->equals(two));
	assert(test->subtract(two)->equals(zero));
	
	EdgeCombination * test2 = four->clone();
	assert(test2->subtract(three)->equals(one));
	
	EdgeCombination * test3 = two->clone();
	assert(two->subtract(one)->equals(one));
	
	delete zero;
	delete one;
	delete two;
	delete three;
	delete four;
	delete five;
	delete six;
	delete seven;
	delete test2;
	delete test;
	delete test3;
	
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

void testIteratorDivide()
{
	cout << "Test iterator divide" << endl;

	EdgeCombination * zero = new EdgeCombination(4);
	EdgeCombination * one = new EdgeCombination(4);
	one->setColor(0, YELLOW);
	EdgeCombination * two = new EdgeCombination(4);
	two->setColor(1, YELLOW);
	EdgeCombination * three = new EdgeCombination(4);
	three->setColor(0, YELLOW);
	three->setColor(1, YELLOW);
	EdgeCombination * four = new EdgeCombination(4);
	four->setColor(2, YELLOW);
	EdgeCombination * five = new EdgeCombination(4);
	five->setColor(0, YELLOW);
	five->setColor(2, YELLOW);
	EdgeCombination * six = new EdgeCombination(4);
	six->setColor(1, YELLOW);
	six->setColor(2, YELLOW);
	EdgeCombination * seven = new EdgeCombination(4);
	seven->setColor(0, YELLOW);
	seven->setColor(1, YELLOW);
	seven->setColor(2, YELLOW);
	EdgeCombination * eight = new EdgeCombination(4);
	eight->setColor(3, YELLOW);
	EdgeCombination * fifteen = new EdgeCombination(4);
	fifteen->setColor(0, YELLOW);
	fifteen->setColor(1, YELLOW);
	fifteen->setColor(2, YELLOW);
	fifteen->setColor(3, YELLOW);
	
	CombinationIterator * tmp = new CombinationIterator(zero->clone(), seven->clone());
	
	vector<CombinationIterator *> intervals = tmp->divide(3);
	assert(intervals.size() == 3);
	assert(intervals[0]->getCurrent()->equals(zero));
	assert(intervals[0]->getMax()->equals(one));
	assert(intervals[1]->getCurrent()->equals(two));
	assert(intervals[1]->getMax()->equals(three));
	assert(intervals[2]->getCurrent()->equals(four));
	assert(intervals[2]->getMax()->equals(seven));

	vector<CombinationIterator *> intervals2 = tmp->divide(2);
	assert(intervals2.size() == 2);
	assert(intervals2[0]->getCurrent()->equals(zero));
	assert(intervals2[0]->getMax()->equals(three));
	assert(intervals2[1]->getCurrent()->equals(four));
	assert(intervals2[1]->getMax()->equals(seven));

	vector<CombinationIterator *> intervals3 = tmp->divide(1);
	assert(intervals3.size() == 1);
	assert(intervals3[0]->getCurrent()->equals(zero));
	assert(intervals3[0]->getMax()->equals(seven));
	
	CombinationIterator * tmp2 = new CombinationIterator(zero->clone(), fifteen->clone());
	
	vector<CombinationIterator *> intervals4 = tmp2->divide(3);
	assert(intervals4.size() == 3);
	assert(intervals4[0]->getCurrent()->equals(zero));
	assert(intervals4[0]->getMax()->equals(three));
	assert(intervals4[1]->getCurrent()->equals(four));
	assert(intervals4[1]->getMax()->equals(seven));
	assert(intervals4[2]->getCurrent()->equals(eight));
	assert(intervals4[2]->getMax()->equals(fifteen));

	delete tmp;
	delete tmp2;
	delete zero;
	delete one;
	delete two;
	delete three;
	delete four;
	delete five;
	delete six;
	delete seven;
	delete eight;
	delete fifteen;

	cout << endl;
}

int main (int argc, char const* argv[])
{
	cout << "Graph tests" << endl << endl;
	testAddAndHasEdge();
	testEdgeCount();
	testTestTriangle();
	testGetNeighbours();
	
	cout << endl << "Factory tests" << endl << endl;
	testFactory();
	
	cout << endl << "Combination tests" << endl << endl;
	testCombination();
	testCombinationIncrement();
	testCombinationAdd();
	testCombinationSubtract();
	testCombinationDivideBy2();
	testCombinationOperators();
	testCombinationCounts();
	testCombinationClone();
	
	cout << endl << "Iterator tests" << endl << endl;
	testIterator();
	testIteratorDivide();
	
	return 0;
}
