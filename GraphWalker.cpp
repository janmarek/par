#include <stack>
#include <deque>
#include <iostream>
#include "Graph.h"
#include "GraphWalker.h"
#include "Solution.h"
#include "EdgeCombination.h"

Solution * GraphWalker::process(Graph * graph) const
{
	return new Solution(new Graph(3), new EdgeCombination(3));
}


