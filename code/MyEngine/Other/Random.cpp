#include "pch.h"
#include "Random.h"

#include <cstdlib>

#include "Math/Scalars.h"

using namespace MyEngine;

void Random::Seed()
{
	srand(static_cast<unsigned>(time(nullptr)));
}

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

int Random::Range(int last)
{
	return std::rand() % (last + 1);
}

Float2 Random::Range(const RectFloat& rect)
{
	return { Range(rect.GetLeft(), rect.GetRight()), Range(rect.GetBottom(), rect.GetTop()) };
}

Float3 Random::Color()
{
	return { Range(1.f), Range(1.f) , Range(1.f) };
}

float Random::Angle()
{
	return Range(Constants::PI2);
}

Float2 Random::UnitVector2()
{
	const float angle{ Angle() };
	return { cosf(angle), sinf(angle) };
}

Float3 Random::UnitVector3()
{
	return Float3{
		Range(-1.1f, 1.f),
		Range(-1.1f, 1.f),
		Range(-1.1f, 1.f)
	}.NormalizedSafe();
}
