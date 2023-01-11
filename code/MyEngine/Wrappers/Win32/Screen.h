#pragma once
#include "Math/RectInt.h"

namespace MyEngine
{
	namespace Wrappers
	{
		namespace Win32
		{
			class Screen
			{
			private:
				static Screen m_Instance;
				Math::RectInt m_Rect;
				Screen();

			public:
				static const Screen& GetInstance() { return m_Instance; }
				const Math::RectInt& GetRect() const { return m_Rect; }
			};
		}
	}
}
