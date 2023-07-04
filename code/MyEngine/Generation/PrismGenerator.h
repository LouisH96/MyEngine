#pragma once
#include "DataStructures/List.h"
#include "Rendering/Structs/VertexTypes.h"
#include <Geometry/Shapes/Rects.h>

namespace MyEngine
{
	struct Quad;

	namespace Generation
	{
		class PrismGenerator
		{
		public:
			static void Create(
				const Quad& quadBot, const Quad& quadTop, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void ConnectSides(
				const Quad& quadBot, const Quad& quadTop, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});

			static void ConnectBack(
				const Quad& quadBot, const Quad& quadTop, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void ConnectFront(
				const Quad& quadBot, const Quad& quadTop, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void ConnectLeft(
				const Quad& quadBot, const Quad& quadTop, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void ConnectRight(
				const Quad& quadBot, const Quad& quadTop, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void ConnectTop(
				const Quad& quadBot, const Quad& quadTop, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});
			static void ConnectBot(
				const Quad& quadBot, const Quad& quadTop, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices, const Float3& offset = {});

			static void Connect(
				const RectFloat* pRects, const float* pHeightSteps, unsigned nrRects, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices,
				const Float3& offset = {}, bool capBegin = false, bool capEnd = false);
		};
	}
}
