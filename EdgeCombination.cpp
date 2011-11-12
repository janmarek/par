#include "EdgeCombination.h"
#include "Color.cpp"

EdgeCombination::EdgeCombination(int size, Color * combination)
{
	this->size = size;
	this->combination = new Color[size];
	
	for (int i = 0; i < size; i++) {
		setColor(size - 1 - i, combination[i]);
	}
}

EdgeCombination::EdgeCombination(int size)
{
	this->size = size;	
	combination = new Color[size];
	
	for (int i = 0; i < size; i++) {
		combination[i] = RED;
	}
}

EdgeCombination::~EdgeCombination()
{
	delete [] combination;
}

int EdgeCombination::getSize() const
{
	return size;
}

// overloaded combination++ operator
const EdgeCombination * EdgeCombination::operator++(int)
{
	for (int i = 0; i < size; i++) {
		if (isRed(i)) {
			setColor(i, YELLOW);
			break;
		} else {
			setColor(i, RED);
		}
	}
	return this;
}
	
bool EdgeCombination::operator==(const EdgeCombination &c) const
{
	if (this->size != c.size) {
		return false;
	}
	
	for (int i = 0; i < size; i++)
	{
		if (this->combination[i] != c.combination[i]) {
			return false;
		}
	}
	
	return true;
}

bool EdgeCombination::operator!=(const EdgeCombination &c) const
{
	if (this->size != c.size) {
		return true;
	}
	
	for (int i = 0; i < size; i++)
	{
		if (this->combination[i] != c.combination[i]) {
			return true;
		}
	}
	
	return false;
}

bool EdgeCombination::isRed(int edge) const
{
	return getColor(edge) == RED;
}

bool EdgeCombination::isYellow(int edge) const
{
	return getColor(edge) == YELLOW;
}

Color EdgeCombination::getColor(int edge) const
{
	return combination[edge];
}

void EdgeCombination::setColor(int edge, Color color)
{
	combination[edge] = color;
}
