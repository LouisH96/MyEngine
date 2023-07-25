#pragma once

namespace MyEngine
{
	namespace Physics
	{
		class LineCubeCollision
		{
		public:
			static bool Detect(
				const CubeAA& cube,
				const Line& line
			);
			static bool Detect(
				const CubeAA& cube,
				const Ray& ray
			);

			static bool Detect(
				const Line& line,
				const Cube& cube
			);
		};
	}
}
