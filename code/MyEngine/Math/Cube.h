#pragma once
#include "Float3.h"

namespace MyEngine
{
	namespace Math
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

			const Float3& GetOrigin() const { return m_Origin; }
			const Float3& GetSize() const { return m_Size; }
			float GetWidth() const { return m_Size.x; }
			float GetHeight() const { return m_Size.y; }
			float GetDepth() const { return m_Size.z; }
			Float3 GetRightTopBack() const { return m_Origin + m_Size; }

		private:
			Float3 m_Origin; //left-bottom-front
			Float3 m_Size;
		};
	}
}
