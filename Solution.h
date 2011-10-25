#pragma once
#include <vector>
#include "Solution.h"
#include "Graph.h"

using namespace std;

class Solution
{
public:
	// konstruktor, zalozeni noveho reseni
        Solution(int nil, Solution &copySol);
        Solution();
        void AddRed(int node1, int node2);
        void AddYellow(int node1, int node2);
        bool CheckTriangleRed(int node1, int node2, Graph * g);
        bool CheckTriangleYellow(int node1, int node2, Graph * g);
        bool stillValid();
        void setValid(bool valid);
        int edgesDiff();
private:
    vector< vector<int> > red;
    vector< vector<int> > yellow;
    vector<int> redNodes;
    vector<int> yelloNodes;
    bool stillValidBool;    
};
