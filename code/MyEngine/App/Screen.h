#pragma once
#include "Math/RectInt.h"

namespace MyEngine
{
	namespace App
	{
		class Screen
		{
		private:
			Math::RectInt m_Rect;

			Screen();

		public:
			static Screen Instance;
			const Math::RectInt& GetRect() const { return m_Rect; }
		};
	}
}
