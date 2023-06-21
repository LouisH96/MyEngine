#pragma once
#include "DataStructures/List.h"
#include "Math/Rects.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace Rendering
	{
		class Mesh;
	}
	namespace Generation
	{
		class PlaneGeneration
		{
		public:
			static Array<Rendering::V_Pos2Uv> CreateVertices(const Float2& leftBottom, const Float2& size, const RectFloat& uvs);

			static Array<Rendering::V_PosUv> TowardsZMin(const Float3& leftBottom, const Float2& size);
			static Rendering::Mesh* MeshTowardsZMin(const Float3& leftBottom, const Float2& size);

			static void TowardXMin(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardXMax(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardYMin(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardYMax(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardZMin(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardZMax(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);

			static void Create(
				const Float3& right, const Float3& up,
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void Create(
				const Float3& right, const Float3& up, const Float3& normal,
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
		};
	}
}
