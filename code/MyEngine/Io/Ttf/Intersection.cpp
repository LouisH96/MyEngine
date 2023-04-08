#include "pch.h"
#include "Intersection.h"

bool Io::Ttf::Intersection::operator<(const Intersection& other) const
{
	return distance < other.distance;
}

bool Io::Ttf::Intersection::operator>(const Intersection& other) const
{
	return distance > other.distance;
}