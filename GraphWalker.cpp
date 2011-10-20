#include <stack>
#include <deque>
#include <iostream>
#include "Graph.h"
#include "GraphWalker.h"

Result * GraphWalker::process(Graph * graph) const
{
	stack<int> stack;
	stack.push(0);
	
	while (!stack.empty())
	{
		int node = stack.top();
		stack.pop(); // remove node from top
		vector<int> neighbours = graph->getNeighbours(node);
		for (unsigned int i = 0; i < neighbours.size(); i++)
		{
			int currentChild = neighbours[i];
			
			if (graph->isEdgeFresh(node, currentChild))
			{
				graph->setEdgeFresh(node, currentChild, false);
				stack.push(currentChild);
			}
		}
		
		// do some action
		std::cout << "at node " << node << std::endl;
	}
	
	// todo replace with real value
	return new Result(new Graph(3), new Graph(3));
}


