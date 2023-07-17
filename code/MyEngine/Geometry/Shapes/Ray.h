#pragma once

namespace MyEngine
{
	class Ray
	{
	public:
		Ray() = default;

		Float3 Origin;
		Float3 Direction;
		float Length;

		Float3 GetPoint(float distance) const;
	};
}
