#include <stdlib.h>
#include "Graph.h"
#include "Result.h"

Result::Result(Graph * g1, Graph * g2)
{
	firstGraph = g1;
	secondGraph = g2;
}

Result::~Result()
{
	delete firstGraph;
	delete secondGraph;
}

Graph * Result::getFirstGraph() const
{
	return firstGraph;
}

Graph * Result::getSecondGraph() const
{
	return firstGraph;
}

int Result::getPrice() const
{
	// todo check triangles
	return abs(firstGraph->getEdgeCount() - secondGraph->getEdgeCount());
}
