#pragma once
#include <vector>
#include "EdgeCombination.h"

class CombinationIterator
{	
public:
	CombinationIterator(EdgeCombination * start, EdgeCombination * end);
	~CombinationIterator();
	
	EdgeCombination * getCurrent() const;
	EdgeCombination * next();
	bool hasNext() const;
	CombinationIterator * divide();
	std::vector<CombinationIterator *> divide(unsigned int) const;
	EdgeCombination * getMax() const;
	
private:
	EdgeCombination * current;
	EdgeCombination * end;
};

