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
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void BackPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void LeftPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void RightPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void TopPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void BottomPlane(
				const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});

			static void Generate(const Cube& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
		};
	}
}
