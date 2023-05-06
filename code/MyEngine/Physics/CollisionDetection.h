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

			/**
			 * \param normals Triangle normals
			 */
			static bool Detect(const Math::Float3& from, const Math::Float3& to,
				const Array<Math::Float3>& vertices, const Array<Math::Float3>& normals, const Array<int>& indices,
				Collision& collision);

			static bool IsPlanePointInTriangle(const Math::Float3& point,
				const Math::Float3& v0, const Math::Float3& v1, const Math::Float3& v2,
				const Math::Float3& triangleNormal);
		};
	}
}
