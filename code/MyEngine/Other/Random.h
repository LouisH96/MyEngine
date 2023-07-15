#pragma once
#include "Geometry/Shapes/Rects.h"
#include "Math/Vectors.h"

namespace MyEngine
{
	class Random
	{
	public:
		//min & max included
		static float Ratio();
		static float Range(float max);
		static float Range(float min, float max);
		static Float2 Range(const RectFloat& rect);
	};
}

