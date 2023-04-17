#pragma once

namespace MyEngine
{
	namespace Shapes
	{
		class Capsule
		{
		public:
			Capsule();
			Capsule(float length, float radius);
			Capsule(const Math::Float3& bottom, float length, float radius);

			const Math::Float3& GetBottom() const { return m_Bottom; }
			float GetLength() const { return m_Length; }
			float GetRadius() const { return m_Radius; }

		private:
			Math::Float3 m_Bottom;
			float m_Length;
			float m_Radius;
		};
	}
}
