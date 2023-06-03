#pragma once
#include "Float2.h"

namespace MyEngine
{
	namespace Math
	{
		class Rect
		{
		public:
			Rect();
			Rect(const Float2& leftBot, const Float2& size);
			Rect(const Float2& leftBot, float width, float height);
			Rect(float left, float bot, const Float2& size);

			const Float2& GetLeftBot() const { return m_LeftBot; }
			const Float2& GetSize() const { return m_Size; }
			Float2 GetLeftTop() const;
			Float2 GetRightBot() const;
			Float2 GetRightTop() const;

		private:
			Float2 m_LeftBot;
			Float2 m_Size;
		};
	}
}
