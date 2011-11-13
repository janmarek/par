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

EdgeCombination::EdgeCombination(const EdgeCombination & copied)
{
	this->size = copied.size;
	this->combination = new Color[copied.size];
	
	for (int i = 0; i < copied.size; i++) {
		this->setColor(i, copied.getColor(i));
	}
}

EdgeCombination * EdgeCombination::createMaxCombination(int size)
{
	EdgeCombination * c = new EdgeCombination(size);
	
	for (int i = 0; i < size; i++)
	{
		c->setColor(i, YELLOW);
	}
	
	return c;
}

EdgeCombination * EdgeCombination::clone() const
{
	return new EdgeCombination(*this);
}

EdgeCombination::~EdgeCombination()
{
	delete [] combination;
}

int EdgeCombination::getSize() const
{
	return size;
}

void EdgeCombination::increment()
{
	for (int i = 0; i < size; i++) {
		if (isRed(i)) {
			setColor(i, YELLOW);
			break;
		} else {
			setColor(i, RED);
		}
	}
}
	
bool EdgeCombination::equals(const EdgeCombination * c) const
{
	if (this->size != c->size) {
		return false;
	}
	
	for (int i = 0; i < this->size; i++)
	{
		if (this->combination[i] != c->combination[i]) {
			return false;
		}
	}
	
	return true;
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

int EdgeCombination::getYellowCount() const
{
	int count = 0;
	
	for (int i = 0; i < size; i++) {
		if (isYellow(i)) {
			count++;
		}
	}
	
	return count;
}

int EdgeCombination::getRedCount() const
{
	return size - getYellowCount();
}
