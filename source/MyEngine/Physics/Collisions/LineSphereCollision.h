#pragma once
#include "Geometry/Shapes/Sphere.h"

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

			static int Detect(
				const Ray& ray,
				PtrRangeConst<Sphere> spheres);

			template<typename PtrRange>
			static int Detect(
				const Ray& ray,
				PtrRange spheres);
		};

		template <typename PtrRange>
		int LineSphereCollision::Detect(const Ray& ray, PtrRange spheres)
		{
			int idx{ -1 };
			float closest{ Float::Max() };
			for (unsigned i = 0; i < spheres.count; i++)
			{
				const Sphere& sphere{ spheres[i] };
				const Float3 toCenter{ sphere.GetCenter() - ray.Origin };
				const float dot{ ray.Direction.Dot(toCenter) };
				if (dot < 0 || dot > ray.Length) continue;
				const Float3 closestPoint{ ray.GetPoint(dot) };
				const float distanceSq{ closestPoint.DistanceSq(sphere.GetCenter()) };
				if (distanceSq > sphere.GetRadiusSq()) continue;
				const float distance{ sqrtf(distanceSq) };
				const float angleSin{ distance / sphere.GetRadius() };
				const float angleCos{ cosf(asinf(angleSin)) };
				const float distanceInside{ angleCos * sphere.GetRadius() };
				const float t{ dot - distanceInside };
				if (t >= closest) continue;
				idx = i;
				closest = t;
			}
			return idx;
		}
	}
}
