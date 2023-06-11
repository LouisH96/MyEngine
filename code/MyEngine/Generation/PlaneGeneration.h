#pragma once
#include "Math/RectFloat.h"
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
		};
	}
}
