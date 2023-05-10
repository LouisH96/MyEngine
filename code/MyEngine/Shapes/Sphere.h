#pragma once

namespace MyEngine
{
	namespace Shapes
	{
		class Sphere
		{
		public:
			Sphere(const Math::Float3& center, float radius);

			void SetCenter(const Math::Float3& center);
			void SetRadius(float radius);
			const Math::Float3& GetCenter() const { return m_Center; }
			float GetRadius() const { return m_Radius; }

		private:
			Math::Float3 m_Center;
			float m_Radius;
		};
	}
}
