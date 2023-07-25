#pragma once

namespace MyEngine
{
	namespace Physics
	{
		class LineCubeCollision
		{
		public:
			static bool Detect(
				const Line& line,
				const CubeAA& cube
			);
			static bool Detect(
				const Line& line,
				const Cube& cube
			);

			static bool Detect(
				const Ray& ray,
				const CubeAA& cube
			);
		};
	}
}
