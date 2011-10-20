#pragma once
#include <iostream>
#include "Graph.h"

class GraphFactory
{	
public:
	Graph * createFromStream(std::istream & stream) const;
};

