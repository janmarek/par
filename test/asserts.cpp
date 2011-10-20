#include <iostream>

void assert(bool condition)
{
	std::cout << (condition ? '.' : 'F');
}

void assertTrue(bool condition)
{
	assert(condition == true);
}

void assertFalse(bool condition)
{
	assert(condition == false);
}
