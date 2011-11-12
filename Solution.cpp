#include <stdlib.h>
#include "Graph.h"
#include "Solution.h"
#include "EdgeCombination.h"

Solution::Solution(Graph * g, EdgeCombination * c)
{
	graph = g;
	combination = c;
}

Solution::~Solution()
{
	delete graph;
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
	// TODO
}
