#include "Graph.h"
#include <vector>

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
	int count = 0;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (map[i][j]) {
				count++;
			}
		}
	}

	return count / 2;
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

