#pragma once
#include "Float3.h"

namespace MyEngine
{
	namespace Math
	{
		class Sphere
		{
		public:
			Sphere(const Float3& center, float radius);

			void SetCenter(const Float3& center);
			void SetRadius(float radius);
			const Float3& GetCenter() const { return m_Center; }
			float GetRadius() const { return m_Radius; }

		private:
			Float3 m_Center;
			float m_Radius;
		};
	}
}
