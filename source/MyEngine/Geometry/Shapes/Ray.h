#pragma once

namespace MyEngine
{
	class Ray
	{
	public:
		Ray() = default;
		Ray(const Float3& origin, const Float3& direction, float length);
		Ray(const Float3& from, const Float3& to);

		Float3 Origin;
		Float3 Direction;
		float Length{};

		Float3 GetPoint(float distance) const;
		Float3 GetEnd() const;
	};
}
