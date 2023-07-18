#pragma once

namespace MyEngine
{
	namespace Physics
	{
		class LinePlaneCollision
		{
		public:
			static bool DetectXz(
				const Ray& ray,
				float planeY,
				Float2& hitPoint);
		};
	}
}
