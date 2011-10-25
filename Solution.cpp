#include <stdlib.h>

#include "Solution.h"
#include "Graph.h"

Solution::Solution(int nil, Solution &copySol) 
{
    for (unsigned int i = 0; i < copySol.red.size(); i++)
    {
        this->red.push_back(copySol.red[i]);
    }
    for (unsigned int i = 0; i < copySol.redNodes.size(); i++)
    {
        this->redNodes.push_back(copySol.redNodes[i]);
    }
    for (unsigned  int i = 0; i < copySol.yelloNodes.size(); i++)
    {
        this->yelloNodes.push_back(copySol.yelloNodes[i]);
    }
    for (unsigned int i = 0; i < copySol.yellow.size(); i++)
    {
        this->yellow.push_back(copySol.yellow[i]);
    }
	this->stillValidBool = true;
}

Solution::Solution()
{
	this->stillValidBool = true;
}

void Solution::AddRed(int node1, int node2)
{  
    vector<int> vc;
    vc.push_back(node1);
    vc.push_back(node2);
    this->red.push_back(vc);
    this->redNodes.push_back(node1);
    this->redNodes.push_back(node2);
}

void Solution::AddYellow(int node1, int node2)
{
    vector<int> vc;
    vc.push_back(node1);
    vc.push_back(node2);
    this->yellow.push_back(vc);
    this->yelloNodes.push_back(node1);
    this->yelloNodes.push_back(node2);
}

bool Solution::CheckTriangleRed(int node1, int node2, Graph * g)
{
    for (unsigned int i = 0; i < this->redNodes.size(); i++)
    {
		
        if (g->testTriangle(node1, node2, this->redNodes[i]))
        {
            return true;
        }
    }
    return false;
}

bool Solution::CheckTriangleYellow(int node1, int node2, Graph * g)
{
    for (unsigned int i = 0; i < this->yelloNodes.size(); i++)
    {     
        if (g->testTriangle(node1, node2, this->yelloNodes[i]))
        {
            return true;
        }
    }
    return false;
}
bool Solution::stillValid()
{
    return this->stillValidBool; 
    return true;
}
void Solution::setValid(bool valid)
{
   // this->stillValidBool = valid;
}
int Solution::edgesDiff()
{
    int a = this->red.size() - this->yellow.size();
    return abs(a);    
}
