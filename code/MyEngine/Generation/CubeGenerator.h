#pragma once
#include "DataStructures/List.h"
#include "Math/Cube.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace Generation
	{
		class CubeGenerator
		{
		public:
			static void FrontPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void BackPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void LeftPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void RightPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TopPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void BottomPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);

			static void Generate(const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
		};
	}
}
