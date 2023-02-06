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

			Int2 operator+ (const Int2& right) const
			{
				return { x + right.x, y + right.y };
			}
			Int2 operator- (const Int2& right) const
			{
				return { x - right.x, y - right.y };
			}
			void operator+= (const Int2& right)
			{
				x += right.x;
				y += right.y;
			}
			void operator-= (const Int2& right)
			{
				x -= right.x;
				y -= right.y;
			}
		};
	}
}
