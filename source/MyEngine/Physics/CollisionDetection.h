#pragma once

namespace MyEngine
{
	class Sphere;

	namespace Physics
	{
		class CollisionDetection
		{
		public:
			struct Collision
			{
				Float3 position;
			};

			static bool Detect(const Float3& from, const Float3& to,
				const Array<Float3>& vertices, const Array<Float3>& triangleNormals,
				Collision& collision);

			static bool Detect(const Ray& ray,
				const Array<Float3>& vertices, const Array<Float3>& triangleNormals,
				Collision& collision);

			static bool Detect(const Float3& from, const Float3& to,
				const Array<Float3>& vertices, const Array<Float3>& triangleNormals, const Array<int>& indices,
				Collision& collision);

			static bool Detect(
				const Sphere& sphere,
				const Array<Float3>& vertices, const Array<Float3>& triangleNormals, const Array<int>& indices,
				Float3& overlap);

			static bool IsLineInTriangle(
				const Float3& l0, const Float3& l1,
				const Float3& t0, const Float3& t1, const Float3& t2,
				const Float3& tNormal,
				Collision& collision);

		private:
			static bool IsPlanePointInTriangle(const Float3& point,
				const Float3& v0, const Float3& v1, const Float3& v2,
				const Float3& triangleNormal);

			static float GetTime(
				const Float3& v0, const Float3& normal,
				const Float3& rayOrigin, const Float3& rayDir);
		};
	}
}
