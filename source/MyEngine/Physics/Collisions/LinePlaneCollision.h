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

			static bool Detect(
				const Ray& ray,
				const Float3& planeOrigin, const Float3& planeRight, const Float3& planeUp,
				Float2& hitPoint);

			//---| Ray |---
			static bool Detect(
				const Float3& origin, const Float3& direction, float length,
				const Float3& planePoint, const Float3& planeNormal,
				Float3& hitPoint);
		};
	}
}
