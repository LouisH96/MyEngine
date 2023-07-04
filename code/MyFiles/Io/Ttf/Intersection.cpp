#include "Intersection.h"

bool MyEngine::Io::Ttf::Intersection::operator<(const Intersection& other) const
{
	return distance < other.distance;
}

bool MyEngine::Io::Ttf::Intersection::operator>(const Intersection& other) const
{
	return distance > other.distance;
}