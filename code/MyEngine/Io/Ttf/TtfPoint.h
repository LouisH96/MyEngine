#pragma once

#include <Math/Value2.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			struct TtfPoint
			{
				Math::Value2<int16_t> position{};
				bool isOnCurve{};
			};
		}
	}
}
