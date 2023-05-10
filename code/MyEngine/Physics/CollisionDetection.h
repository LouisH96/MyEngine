#pragma once

namespace MyEngine
{
	namespace Physics
	{
		class CollisionDetection
		{
		public:
			struct Collision
			{
				Math::Float3 position;
			};

			static bool Detect(const Math::Float3& from, const Math::Float3& to,
				const Array<Math::Float3>& vertices, const Array<Math::Float3>& triangleNormals, 
				Collision& collision);

			static bool Detect(const Math::Float3& from, const Math::Float3& to,
				const Array<Math::Float3>& vertices, const Array<Math::Float3>& triangleNormals, const Array<int>& indices,
				Collision& collision);

			static bool Detect(
				const Sphere& sphere,
				const Array<Float3>& vertices, const Array<Float3>& triangleNormals, const Array<int>& indices,
				Float3& overlap);

		private:
			static bool IsPlanePointInTriangle(const Math::Float3& point,
				const Math::Float3& v0, const Math::Float3& v1, const Math::Float3& v2,
				const Math::Float3& triangleNormal);

			static float GetTime(
				const Math::Float3& v0, const Math::Float3& normal,
				const Math::Float3& rayOrigin, const Math::Float3& rayDir);
		};
	}
}
