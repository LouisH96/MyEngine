#pragma once

namespace MyEngine
{
	struct Line;
}

namespace MyEngine
{
	namespace Physics
	{
		class LineOnLineCollision
		{
		public:

			//returns T of line A
			static bool Detect(
				const Line& lineA,
				const Line& lineB,
				Float3& hitPoint);
		};
	}
}
