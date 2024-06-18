#pragma once

namespace MyEngine
{
	class Cone
	{
	public:
		Cone() = default;
		Cone(const Float3& origin, const Float3& direction, float length, float radius);

		Float3 origin;
		Float3 direction;
		float length;
		float radius;

		Float3 GetEnd() const;
		Float3 GetRight() const;
		Float3 GetUp(const Float3& right) const;
		float GetAngle() const;
	};
}
