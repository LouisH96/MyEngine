#pragma once
#include "Math/Vectors.h"

namespace MyEngine
{
	class Cube
	{
	public:
		Cube();
		Cube(Float3 origin, Float3 size);
		Cube(Float3 origin, float size);
		~Cube() = default;
		Cube(const Cube& other) = default;
		Cube(Cube&& other) noexcept = default;
		Cube& operator=(const Cube& other) = default;
		Cube& operator=(Cube&& other) noexcept = default;

		Float3& GetOrigin() { return m_Origin; }
		const Float3& GetOrigin() const { return m_Origin; }
		const Float3& GetSize() const { return m_Size; }
		float GetWidth() const { return m_Size.x; }
		float GetHeight() const { return m_Size.y; }
		float GetDepth() const { return m_Size.z; }

		const Float3& GetLeftBotBack() const { return m_Origin; } //back = -z
		Float3 GetLeftBotFront() const { return m_Origin + Float3{0, 0, m_Size.z}; }
		Float3 GetLeftTopBack() const { return m_Origin + Float3{0, m_Size.y, 0}; }
		Float3 GetRightBotBack() const { return m_Origin + Float3{m_Size.x, 0, 0}; };
		Float3 GetRightBotFront() const { return m_Origin + Float3{m_Size.x, 0, m_Size.z}; }
		Float3 GetRightTopFront() const { return m_Origin + m_Size; }

		float GetLeft() const { return m_Origin.x; }
		float GetTop() const { return m_Origin.y + m_Size.y; }
		float GetBack() const { return m_Origin.z; }

		Float2 GetXySize() const { return { m_Size.x, m_Size.y }; }
		Float2 GetXzSize() const { return { m_Size.x, m_Size.z }; }
		Float2 GetZySize() const { return { m_Size.z, m_Size.y }; }

		static constexpr unsigned NR_SIDES = 6;

	private:
		Float3 m_Origin; //left-bottom-back
		Float3 m_Size;
	};
}
