#pragma once

namespace MyEngine
{
	namespace Math
	{
		union Int2
		{
		public:
			struct { int x; int y; };
			struct { int width; int height; };
			int value[2];

			Int2(int x, int y);
		};
	}
}
