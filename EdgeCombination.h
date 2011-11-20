#pragma once
#include "Color.cpp"

class EdgeCombination
{
public:
	// konstruktor, size je velikost grafu, EdgeCombination
	EdgeCombination(int size, Color * combination);
	
	// začne na první kombinace (všechny hrany RED)
	EdgeCombination(int size);
	
        EdgeCombination(int size, int edgeYellow);
        
	EdgeCombination(const EdgeCombination &);
	
	~EdgeCombination();
	
	EdgeCombination * clone() const;
	
	static EdgeCombination * createMaxCombination(int size);
	
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
