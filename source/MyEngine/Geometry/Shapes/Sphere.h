#pragma once

#include <Math/Vectors.h>

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

	private:
		Float3 m_Center;
		float m_Radius;
	};
}
