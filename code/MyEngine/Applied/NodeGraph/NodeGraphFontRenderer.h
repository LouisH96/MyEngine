#pragma once
#include "Font/TextAssembler.h"
#include "Rendering/Buffers/Buffer.h"

namespace MyEngine
{
	namespace Applied
	{
		class NodeGraphFontRenderer
		{
		public:
			using Vertex = Rendering::V_Pos2ColUv;

			NodeGraphFontRenderer();

			void Render();

		private:
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;

			Rendering::Texture m_FontAtlas;
			Rendering::DepthStencilState m_DepthStencilState;
			Rendering::SamplerState m_SamplerState;
			Rendering::BlendState m_BlendState;
			Rendering::RasterizerState m_RasterizerState;

			Rendering::Buffer<Vertex> m_Vertices;

			TextAssembler m_Assembler;
		};
	}
}
