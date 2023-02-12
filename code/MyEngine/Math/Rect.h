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

		private:
			Float2 m_LeftBot;
			Float2 m_Size;
		};
	}
}
