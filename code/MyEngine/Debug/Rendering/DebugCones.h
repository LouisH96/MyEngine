#pragma once
#include "Rendering/DrawData/VertexIdxList.h"

namespace MyEngine
{
	class Cone;

	namespace Debug
	{
		class DebugCones
		{
		public:
			DebugCones();

			void Render();

			void Draw(const Cone& cone, const Float3& color);

		private:
			using Vertex = Rendering::V_PosColNorm;
			Rendering::VertexIdxList<Vertex> m_Vertices;
		};
	}
}
