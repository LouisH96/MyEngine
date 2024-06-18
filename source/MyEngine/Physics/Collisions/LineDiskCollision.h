#pragma once

namespace MyEngine
{
	namespace Physics
	{
		class LineDiskCollision
		{
		public:
			static bool Detect(const Ray& ray, const Float3& diskCenter, const Float3& diskNormal, float diskRadiusSq, float& t);
		};
	}
}
