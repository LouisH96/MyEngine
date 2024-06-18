#pragma once
#include "Rendering/DrawData/VertexIdxList.h"

namespace MyEngine
{
	class Cube;
	class CubeAA;
}

namespace MyEngine
{
	namespace Debug
	{
		class DebugCubes
		{
		public:
			DebugCubes();

			void Render();

			void Draw(const Cube& cube, const Float3& color);
			void Draw(const CubeAA& cube, const Float3& color);

		private:
			using Vertex = Rendering::V_PosColNorm;

			Rendering::VertexIdxList<Vertex> m_Vertices;
		};
	}
}
