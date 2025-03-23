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

	//---| Origin Aligned Box |---
	static bool Detect(
		const Ray& ray, const Float3& invDirection,
		const Float3& boxSize);

	static bool Detect(
		const Float3& origin, const Float3& direction, float length,
		const Float3& boxSize);

	static bool Detect(
		const Float3& origin, const Float3& direction, float length,
		const Float3& invDirection,
		const Float3& boxSize);

};
}
}
