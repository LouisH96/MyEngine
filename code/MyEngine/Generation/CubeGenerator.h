#pragma once
#include "DataStructures/List.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	class CubeAA;

	namespace Generation
	{
		class CubeGenerator
		{
		public:
			static void FrontPlane(
				const CubeAA& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void BackPlane(
				const CubeAA& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void LeftPlane(
				const CubeAA& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void RightPlane(
				const CubeAA& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void TopPlane(
				const CubeAA& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void BottomPlane(
				const CubeAA& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});

			static void Generate(const CubeAA& cube, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
		};
	}
}
