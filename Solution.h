#pragma once
#include "Solution.h"
#include "EdgeCombination.h"

/**
 * Result of algorithm
 */
class Solution
{	
public:
	// Constructor takes two graphs (two parts of original graph)
	Solution(Graph *, EdgeCombination *);
	
	~Solution();
	
	// Graph getter
	Graph * getGraph(void) const;
	
	// Graph getter
	EdgeCombination * getCombination(void) const;
	
	// Get price of result (difference between edge count in graphs)
	int getPrice() const;
	
	void print() const;
private:
	Graph * graph;
	EdgeCombination * combination;
};

