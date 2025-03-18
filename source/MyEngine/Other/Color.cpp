#include "pch.h"

using namespace Color;

const Float3& Functions::GetColor(unsigned id)
{
	unsigned count{ 0 };
	if (count++ == id) return Red;
	if (count++ == id) return Green;
	if (count++ == id) return Blue;
	if (count++ == id) return Yellow;
	if (count++ == id) return Chill::Red;
	if (count++ == id) return Chill::Green;
	if (count++ == id) return Chill::Blue;
	if (count++ == id) return Chill::Yellow;
	return GetColor(id % count);
}
