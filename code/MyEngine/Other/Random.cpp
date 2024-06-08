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

Float3 Random::Color(const Float3& base, float deviation)
{
	deviation /= 2;

	Float3 min, max;

	for (unsigned i = 0; i < 3; i++)
	{
		if (base[i] < deviation)
		{
			min[i] = 0;
			max[i] = deviation * 2;
		}
		else if (base[i] + deviation > 1.f)
		{
			max[i] = 1.f;
			min[i] = 1.f - deviation * 2;
		}
		else
		{
			min[i] = base[i] - deviation;
			max[i] = base[i] + deviation;
		}
	}

	return Float3{
		Range(min[0], max[0]),
		Range(min[1], max[1]),
		Range(min[2], max[2])
	};
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
