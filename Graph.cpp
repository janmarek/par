#include "Graph.h"
#include <vector>

using namespace std;

Graph::Graph(int size)
{
	this->size = size;

	vector<int> row(size, 0);
	vector< vector<int> > map(size, row);
	this->map = map;
}

void Graph::addEdge(int node1, int node2)
{
	this->map[node1][node2] = 1;
	this->map[node2][node1] = 1;
}

bool Graph::hasEdge(int node1, int node2) const
{
	return this->map[node1][node2];
}

void Graph::removeEdge(int node1, int node2)
{
	this->map[node1][node2] = 0;
	this->map[node2][node1] = 0;
}

int Graph::getEdgeCount()
{
	int count = 0;

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (this->map[i][j] == 1) {
				count++;
			}
		}
	}

	return count / 2;
}

int Graph::getNodesCount()
{
	return this->size;
}


