#pragma once
#include <iostream>
#include "Graph.h"

// vytvari grafy z matice
class GraphFactory
{	
public:
	// metoda na vytvoreni grafu, muze dostat jakykoliv input stream
	// (std. vstup, file input stream, string input stram)
	Graph * createFromStream(std::istream & stream) const;
};

