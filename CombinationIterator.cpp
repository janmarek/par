#include "EdgeCombination.h"
#include "CombinationIterator.h"
#include <vector>
#include <iostream>

using namespace std;

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

EdgeCombination * CombinationIterator::getMax() const
{
	return end;
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

vector<CombinationIterator *> CombinationIterator::divide(unsigned int ct) const
{
	EdgeCombination * intervalSize = end->clone()->subtract(current);

	unsigned int index = 1;
	while (index < ct) {
		intervalSize->divideBy2();
		index = index * 2;
	}

	vector<CombinationIterator *> vc;

	EdgeCombination * start = current->clone();

	for (unsigned int i = 0; i < ct; ++i) {
		EdgeCombination * intervalMin = start;
		EdgeCombination * intervalMax = i == ct - 1 ? end->clone() : start->clone()->add(intervalSize);
		start = intervalMax->clone();
		start->increment();
		vc.push_back(new CombinationIterator(intervalMin, intervalMax));
	}

	delete intervalSize;

	return vc;
}

CombinationIterator::~CombinationIterator()
{
	delete current;
	delete end;
}
