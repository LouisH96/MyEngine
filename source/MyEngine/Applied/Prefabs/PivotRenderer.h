#pragma once
#include "Rendering/Buffers/Buffer.h"
#include "Rendering/Buffers/IdxBuffer.h"

namespace MyEngine
{
	namespace Applied
	{
		class PivotRenderer
		{
		public:
			PivotRenderer();

			void Render();

		private:
			using Vertex = Rendering::V_PosColNorm;
			Rendering::Shader m_Shader;
			Rendering::InputLayout m_InputLayout;
			Rendering::Buffer<Vertex> m_Vertices;
			Rendering::IdxBuffer m_Indices;
		};
	}
}
