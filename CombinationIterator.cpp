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
	return current->isLower(end);
}

CombinationIterator * CombinationIterator::divide()
{
	EdgeCombination * newEnd = end->clone()->subtract(current)->divideBy2()->add(current);
	EdgeCombination * newStart = newEnd->clone();
	newStart->increment();

	CombinationIterator * newIt = new CombinationIterator(newStart, end);
	this->end = newEnd;

	return newIt;
}

CombinationIterator::~CombinationIterator()
{
	delete current;
	delete end;
}
