#pragma once
#include <vector>

using namespace std;

class Graph
{
public:
	// konstruktor, size je velikost grafu
	Graph(int size);
	
	// pridej hranu z uzlu do uzlu
	void addEdge(int, int);
	
	// je pritomna hrana z uzlu do uzlu?
	bool hasEdge(int, int) const;
	
	// odeber hranu z uzlu do uzlu
	void removeEdge(int, int);
	
	// pocet hran v grafu
	int getEdgeCount();
	
	// pocet uzlu v grafu
	int getNodesCount();
	
	// indexy sousedu uzlu
	vector<int> getNeighbours(int node) const;

	// zjisti, jestli tri uzly tvori trojuhelnik
	bool testTriangle(int node1, int node2, int node3) const;
	
	// nastav hranu z uzlu do uzlu (ne)cerstvou
	void setEdgeFresh(int node1, int node2, bool fresh);
	
	// je hrana z uzlu do uzlu cerstva?
	int isEdgeFresh(int node1, int node2) const;

private:
	// matice sousednosti (0 - neni hrana, 1 - cerstva hrana, 2 - necerstva hrana)
	vector< vector<int> > map;
	
	// pocet uzlu grafu
	int size;
};
