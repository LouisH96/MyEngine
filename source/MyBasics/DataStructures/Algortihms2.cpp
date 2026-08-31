#include "Algortihms2.h"
#include <Logger/Logger.h>

namespace MyEngine
{
int Algorithms2::Find(unsigned index,
	const int* a, const unsigned aSize,
	const int* b, const unsigned bSize)
{
	unsigned a0 = 0, a1 = aSize;
	unsigned b0 = 0, b1 = bSize;

	while (true)
	{
		if (a0 == a1) return b[index - a0];
		if (b0 == b1) return a[index - b0];

		const unsigned am{ (a1 - a0) / 2 + a0 };
		const unsigned bm{ (b1 - b0) / 2 + b0 };
		const unsigned countLeft{ 1 + am + bm };

		if (index < countLeft)
		{
			if (a[am] < b[bm])
				b1 = bm; //Remove B-Right
			else
				a1 = am; //Remove A-Right
		}
		else
		{
			if (a[am] < b[bm])
				a0 = am + 1; //Remove A-Left
			else
				b0 = bm + 1; //Remove B-Right
		}
	}
	Logger::Warning("[Algorithms2::Find(i, a, b, ...)] Value at index not found");
	return 0;
}
}
