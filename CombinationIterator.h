#pragma once
#include "EdgeCombination.h"

class CombinationIterator
{	
public:
	CombinationIterator(EdgeCombination * start, EdgeCombination * end);
	~CombinationIterator();
	
	EdgeCombination * getCurrent() const;
	EdgeCombination * next();
	bool hasNext() const;
	
private:
	EdgeCombination * current;
	EdgeCombination * end;
};

