#pragma once
#include "Graph.h"
#include "Result.h"

class GraphWalker
{	
public:
	// core algorithm will be implemented in this method
	Result * process(Graph * graph) const;
};

