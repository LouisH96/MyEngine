#pragma once

namespace MyEngine
{
/*
	Random Algorithms
*/
class Algorithms2
{
public:
	/*
		Find element in the combined array of A and B.
		Both array A and B should be sorted.
		This function acts as if A and B are merged.
	*/
	static int Find(unsigned index,
		const int* a, const unsigned sizeA,
		const int* b, const unsigned sizeB);
};

}