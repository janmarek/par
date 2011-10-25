#include <stack>
#include <deque>
#include <iostream>
#include "Graph.h"
#include "GraphWalker.h"
#include "Solutions.h"
#include "Solution.h"

Solution GraphWalker::process(Graph * graph)
{
    Solutions * solutions = new Solutions(graph);   
    int previousNode = 0;
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
            if (previousNode != node)
            {
                solutions->AddEdge(previousNode, node);
            }
            std::cout << "at node " << node << std::endl;
            previousNode = node;
    }
    // todo replace with real value
    return solutions->bestSolution;    
}


