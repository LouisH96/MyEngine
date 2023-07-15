#include "pch.h"
#include "Random.h"

#include <cstdlib>

#include "Math/Scalars.h"

float Random::Ratio()
{
	return static_cast<float>(std::rand()) / RAND_MAX;
}

float Random::Range(float max)
{
	return static_cast<float>(std::rand()) / RAND_MAX * max;
}

float Random::Range(float min, float max)
{
	return Float::Cast(std::rand()) / RAND_MAX * (max - min) + min;
}

Float2 Random::Range(const RectFloat& rect)
{
	return { Range(rect.GetLeft(), rect.GetRight()), Range(rect.GetBottom(), rect.GetTop()) };
}
