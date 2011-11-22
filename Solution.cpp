#include <iostream>
#include <stdlib.h>
#include "Graph.h"
#include "Solution.h"
#include "EdgeCombination.h"

using namespace std;

Solution::Solution(Graph * g, EdgeCombination * c)
{
	graph = g;
	combination = c;
}

Solution::~Solution()
{
	delete combination;
}

Graph * Solution::getGraph() const
{
	return graph;
}

EdgeCombination * Solution::getCombination() const
{
	return combination;
}

int Solution::getPrice() const
{
	return abs(combination->getRedCount() - combination->getYellowCount());
}

void Solution::print() const
{
	cout << endl << endl;
	cout << "Solution has price " << getPrice() << endl;

	for (int i = 0; i < combination->getSize(); ++i) {
		cout << "* edge from " << graph->getEdge(i)[0] << " to " << graph->getEdge(i)[1] << " is "
			<< (combination->isRed(i) ? "red" : "yellow") << endl;
	}
}
