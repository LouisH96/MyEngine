#include "pch.h"
#include "Line.h"

float Line::GetLength() const
{
	return a.Distance(b);
}

Float3 Line::GetDirection() const
{
	return (b - a).Normalized();
}
