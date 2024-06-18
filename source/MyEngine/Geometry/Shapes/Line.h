#pragma once
#include <Math/Vectors.h>

namespace MyEngine
{
	struct Line
	{
		Float3 a;
		Float3 b;

		float GetLength() const;
		Float3 GetDirection() const;
	};
}
