#pragma once
#include "Physics/Collisions/TriangleCollisionDetection.h"

namespace MyEngine
{
	template<typename Point>
	class CollisionDetection2
	{
	public:
		static bool Detect(const TriangleBase<Point>& firstShape, const TriangleBase<Point>& secondShape);
	};

	template<typename Point>
	inline bool CollisionDetection2<Point>::Detect(const TriangleBase<Point>& firstShape, const TriangleBase<Point>& secondShape)
	{
		return TriangleCollisionDetection<Point>::Detect(firstShape, secondShape);
	}

}