#pragma once

#include <Math/Vector2.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			struct TtfPoint
			{
				Math::Vector2<int16_t> position{};
				bool isOnCurve{};
			};
		}
	}
}
