#pragma once
#include "Geometry/Shapes/Rects.h"
#include "Math/Vectors.h"

namespace MyEngine
{
	class Random
	{
	public:
		static void Seed();

		//min & max included
		static float Ratio();
		static float Range(float max);
		static float Range(float min, float max);
		static int Range(int last);
		static Float2 Range(const RectFloat& rect);
		static Float3 Color();
		static float Angle();
		static Float2 UnitVector2();
		static Float3 UnitVector3();

		template<typename T> static T Enum(T last);
	};

	template <typename T>
	T Random::Enum(T last)
	{
		return static_cast<T>(Range(static_cast<int>(last)));
	}
}

