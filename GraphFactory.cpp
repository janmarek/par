#include <iostream>
#include "Graph.h"
#include "GraphFactory.h"

Graph * GraphFactory::createFromStream(std::istream & stream) const {
	unsigned int size = 3;

	stream >> size;
	
	Graph * graph = new Graph(size);

	char ch;
	stream.get(ch); // new line

	for (unsigned int i = 0; i < size; i++)
	{
		for (unsigned int j = 0; j < size; j++)
		{
			stream.get(ch);
			if (ch == '1') {
				graph->addEdge(i, j);
			}
		}
		stream.get(ch); // new line
	}
	
	return graph;
}


