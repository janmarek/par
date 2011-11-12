#include "EdgeCombination.h"
#include "CombinationIterator.h"

CombinationIterator::CombinationIterator(EdgeCombination * start, EdgeCombination * end)
{
	this->current = start;
	this->end = end;
}

EdgeCombination * CombinationIterator::getCurrent() const
{
	return current;
}

EdgeCombination * CombinationIterator::next()
{
	if (hasNext()) {
		current->increment();
	}
	
	return current;
}

bool CombinationIterator::hasNext() const
{
	return !current->equals(end);
}

CombinationIterator::~CombinationIterator()
{
	delete current;
	delete end;
}
