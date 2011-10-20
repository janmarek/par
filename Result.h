#pragma once
#include "Graph.h"

/**
 * Result of algorithm
 */
class Result
{	
public:
	// Constructor takes two graphs (two parts of original graph)
	Result(Graph *, Graph *);
	
	~Result();
	
	// Graph getter
	Graph * getFirstGraph(void) const;
	
	// Graph getter
	Graph * getSecondGraph(void) const;
	
	// Get price of result (difference between edge count in graphs)
	int getPrice() const;
private:
	Graph * firstGraph;
	Graph * secondGraph;
};

