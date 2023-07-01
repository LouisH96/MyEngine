#pragma once
#include "Rendering/Font/FontAtlasLookup.h"
#include "Rendering/Renderers/NdcRectRenderer.h"
#include "Rendering/State/SamplerState.h"
#include "Rendering/State/Texture.h"
#include "Rendering/Structs/VertexTypes.h"
#include "Rendering/Structs/InstanceTypes.h"

namespace MyEngine
{
	namespace Gui
	{
		class FontRenderer
		{
		public:
			FontRenderer();

			void OnCanvasResized(const App::ResizedEvent& event);
			void Render();

			void Remove(int id);
			int Add(char c, const Float2& pivot, const Float2& offset, float height, const Float3& color);

		private:
			using Vertex = Rendering::V_Pos2Uv;
			using Instance = Rendering::I_Pos2UvCol;

			Rendering::NdcRectRenderer<Vertex, Instance> m_Renderer;
			Rendering::Texture m_FontAtlas;
			Rendering::DepthStencilState m_DepthStencilState;
			Rendering::SamplerState m_SamplerState;

			Rendering::FontAtlasLookup m_CharLookup;
			float m_UvXHeight;
			float m_UvHeightToWidth;
		};
	}
}
