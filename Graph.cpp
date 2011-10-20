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

/**
 * Vrati indexy sousedicich uzlu pro uzel node
 */
vector<int> Graph::getNeighbours(int node) const
{
	vector<int> vc;
	
	for (int i = 0; i < this->size; i++)
	{
		if (this->map[node][i] == 1)
		{
			vc.push_back(i);
		}
	}

	return vc;
}



/**
 * Vrati indexy sousedicich uzlu s vetsim indexem pro uzel node
 */
vector<int> Graph::getNeighboursWithBiggerIndex(int node) const
{
	vector<int> vc;
	
	for (int i = node + 1; i < this->size; i++)
	{
		if (this->map[node][i] == 1)
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
	return this->hasEdge(node1, node2) && this->hasEdge(node2, node3) && this->hasEdge(node1, node3);
}

