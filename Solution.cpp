#include <iostream>
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
    std::cout << "Solution: ";
    for ( int i = 0; i < this->combination->getSize(); i++)
    {
        Color col = this->combination->getColor(i);
        vector<int> edge = this->graph->getEdge(i);
        std::cout << "E" << col << ":" << edge[0] << "," << edge[1] << "; ";
    }
    
	std::cout << std::endl << "Solution has price " << getPrice() << std::endl;
}
