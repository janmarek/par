#pragma once
#include <vector>

using namespace std;

class Graph
{
public:
	Graph(int);
	void addEdge(int, int);
	bool hasEdge(int, int) const;
	void removeEdge(int, int);
	int getEdgeCount();
	int getNodesCount();
	vector<int> getNeighbours(int) const;
	bool testTriangle(int node1, int node2, int node3) const;

private:
	vector< vector<int> > map;
	int size;
};
