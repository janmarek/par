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

private:
	vector< vector<int> > map;
	int size;
};
