#pragma once
#include "Geometry/Shapes/Rects.h"

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Screen
			{
			private:
				static Screen m_Instance;
				RectInt m_Rect;
				Screen();

			public:
				static const Screen& GetInstance() { return m_Instance; }
				const RectInt& GetRect() const { return m_Rect; }
			};
		}
	}
}
