#pragma once

#include <Math/Vectors.h>
#include "CubeAA.h"

namespace MyEngine
{
class Sphere
{
public:
	Sphere() = default;
	Sphere(const Float3& center, float radius);

	void SetCenter(const Float3& center);
	void SetRadius(float radius);
	const Float3& GetCenter() const { return m_Center; }
	Float3& GetCenter() { return m_Center; }
	float GetRadius() const { return m_Radius; }
	float GetRadiusSq() const { return m_Radius * m_Radius; }
	float GetDiameter() const { return m_Radius * 2; }

	Sphere operator+(const Float3& movement) const;
	Sphere operator-(const Float3& movement) const;

	CubeAA GetGlobalBounds(const Float3& direction, float distance) const;

	static CubeAA GetGlobalBounds(
		const Float3& center, float radius,
		const Float3& direction, float distance);

private:
	Float3 m_Center;
	float m_Radius{};
};
}
