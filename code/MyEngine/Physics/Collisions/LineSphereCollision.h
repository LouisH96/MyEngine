#pragma once

namespace MyEngine
{
	class Sphere;
	class Ray;

	namespace Physics
	{
		class LineSphereCollision
		{
		public:
			static bool Detect(
				const Ray& ray,
				const Sphere& sphere);
		};
	}
}
