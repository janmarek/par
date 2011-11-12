#pragma once
#include "Color.cpp"

class EdgeCombination
{
public:
	// konstruktor, size je velikost grafu, EdgeCombination
	EdgeCombination(int size, Color * combination);
	
	~EdgeCombination();
	
	// začne na první kombinace (všechny hrany false)
	EdgeCombination(int size);
	
	int getSize() const;
	
	void increment();
	
	bool equals(const EdgeCombination * c) const;
	
	Color getColor(int edge) const;
	
	void setColor(int edge, Color color);
	
	bool isRed(int edge) const;
	
	bool isYellow(int edge) const;
	
	int getYellowCount() const;
	
	int getRedCount() const;

private:
	// combination
	Color * combination;
	
	// pocet uzlu grafu
	int size;
};
