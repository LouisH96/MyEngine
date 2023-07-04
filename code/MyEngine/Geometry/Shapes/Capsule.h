#pragma once

namespace MyEngine
{
	class Capsule
	{
	public:
		Capsule();
		Capsule(float length, float radius);
		Capsule(const Float3& bottom, float length, float radius);

		const Float3& GetBottom() const { return m_Bottom; }
		float GetLength() const { return m_Length; }
		float GetRadius() const { return m_Radius; }

	private:
		Float3 m_Bottom;
		float m_Length;
		float m_Radius;
	};
}
