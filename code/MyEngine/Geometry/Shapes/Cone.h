#pragma once

namespace MyEngine
{
	namespace Geometry
	{
		class Cone
		{
		public:
			Cone() = default;
			Cone(const Float3& origin, float length, float radius);

			Float3 origin;
			float length;
			float radius;
		};
	}
}
