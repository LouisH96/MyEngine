#pragma once
#include <Rendering/Structs/VertexTypes.h>

namespace MyEngine
{
	class Cube;
	class Sphere;
	class Cube;
	namespace Ds
	{
		template<typename>
		class Array;
	}

	namespace Generation
	{
		class Shapes
		{
		public:
			static void GenerateCubeBuffers(const Cube& cube, Array<Float3>& positions, Array<Float3>& normals, Array<int>& indices);
			static void GenerateSphereBuffers(const Sphere& sphere, int nrCols, int nrRows, Array<Float3>& positions, Array<Float3>& normals, Array<int>& indices);
			static void GeneratePivotArrows(Array<Rendering::V_PosColNorm>& vertices, Array<int>& indices);
			static Array<int> GetIndicesToCapCircle(int nrPoints);
		};
	}
}
