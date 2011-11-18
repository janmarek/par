#include "Graph.h"
#include <vector>
#include "EdgeCombination.h"

using namespace std;

Graph::Graph(int size)
{
	this->size = size;

	vector<bool> row(size, false);
	vector< vector<bool> > map(size, row);
	this->map = map;
}

void Graph::addEdge(int node1, int node2)
{
	map[node1][node2] = true;
	map[node2][node1] = true;
        // souradnice hrany pro snazsi vypocet trojuhelnikovosti;
        this->edgeCount++;
        vector<int> edge(2, 0);
        edge[0] = node1;
        edge[1] = node2;        
        this->edges.push_back(edge);
}
vector<int> Graph::getEdge(int edgeNumber)
{
    return this->edges[edgeNumber];
}

bool Graph::hasEdge(int node1, int node2) const
{
	return map[node1][node2];
}

void Graph::removeEdge(int node1, int node2)
{
	map[node1][node2] = false;
	map[node2][node1] = false;
}

int Graph::getEdgeCount()
{
    return this->edgeCount;
    /*
	int count = 0;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (map[i][j]) {
				count++;
			}
		}
	}

	return count / 2;
     */
}

int Graph::getNodesCount()
{
	return size;
}

/**
 * Vrati indexy sousedicich uzlu pro uzel node
 */
vector<int> Graph::getNeighbours(int node) const
{
	vector<int> vc;
	
	for (int i = 0; i < size; i++)
	{
		if (map[node][i] > 0)
		{
			vc.push_back(i);
		}
	}

	return vc;
}

/**
 * Zjisti, jestli tri zadane uzly tvori trojuhelnik
 */
bool Graph::testTriangle(int node1, int node2, int node3) const
{
	return hasEdge(node1, node2) && hasEdge(node2, node3) && hasEdge(node1, node3);
}

bool Graph::testTriangleOk(EdgeCombination * c) const
{    
    for (unsigned int i = 0; i < this->edges.size(); i++)
    {
        int node1 = edges[i][0];
        int node2 = edges[i][1];
        if (!this->testEdge(node1, node2, c, c->getColor(i), i))      
        {
            return false;
        }
    }
    return true;
}
// otestuje hranu, zda patri do uzloveho trojuhelniku v grafu
bool Graph::testEdge(int node1, int node2, EdgeCombination * c, Color colorToBeTested, unsigned int currEdge) const
{
    vector<int> neigbours = this->getNeighbours(node1);
    for (unsigned int i = 0; i < neigbours.size(); i++)
    {        
        if (neigbours[i] != node2 && this->testTriangle(node1, node2, neigbours[i]))
        {                         
            int stopUpToTwo = 0; // jednu hranu mame, hledame 2 dalsi stejne barvy            
            for (unsigned int j = 0; j < this->edges.size(); j++)
            {
                if (j!= currEdge)
                {
                    if (((edges[j][0] == node1 && edges[j][1] == neigbours[i]) | (edges[j][0] == neigbours[i] && edges[j][1] == node1))  && c->getColor(j) == colorToBeTested)                                            
                    {
                        stopUpToTwo++;
                    }
                    if (((edges[j][0] == node2 && edges[j][1] == neigbours[i]) | (edges[j][0] == neigbours[i] && edges[j][1] == node2)) && c->getColor(j) == colorToBeTested)
                    {
                        stopUpToTwo++;
                    }
                }
                if (stopUpToTwo > 1)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

