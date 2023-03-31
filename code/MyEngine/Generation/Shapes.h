#pragma once
#include "Math/Sphere.h"
#include <Rendering/Structs/VertexTypes.h>

namespace MyEngine
{
	namespace Ds
	{
		template<typename>
		class Array;
	}
	namespace Math
	{
		struct Float3;
		class Cube;
	}
}

using namespace Ds;
using namespace Math;

namespace MyEngine
{
	namespace Generation
	{
		class Shapes
		{
		public:
			static void GenerateCubeBuffers(const Cube& cube, Array<Float3>& positions, Array<Float3>& normals, Array<int>& indices);
			static void GenerateSphereBuffers(const Sphere& sphere, int nrCols, int nrRows, Array<Float3>& positions, Array<Float3>& normals, Array<int>& indices);
			static void GeneratePivotArrows(Array<Rendering::V_PosColNorm>& vertices, Array<int>& indices);
			static Array<int> GetIndicesToCapCircle(int nrPoints);
		private:

		};
	}
}
