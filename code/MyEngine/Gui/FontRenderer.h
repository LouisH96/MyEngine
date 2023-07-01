#pragma once
#include "Rendering/Font/FontAtlasLookup.h"
#include "Rendering/Renderers/NdcRectRenderer.h"
#include "Rendering/State/BlendState.h"
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
			int Add(const std::string& text, const Float2& pivot, const Float2& offset, float height, const Float3& color, float spacing = 0);

			int AddLeftTop(const std::string& text, float height, const Float2& offset, const Float3& color, float spacing = 0);
			int AddLeftCenter(const std::string& text, float height, const Float2& offset, const Float3& color, float spacing = 0);
			int AddLeftBot(const std::string& text, float height, const Float2& offset, const Float3& color, float spacing = 0);

			int AddCenterTop(const std::string& text, float height, const Float2& offset, const Float3& color, float spacing = 0);
			int AddCenter(const std::string& text, float height, const Float2& offset, const Float3& color, float spacing = 0);
			int AddCenterBot(const std::string& text, float height, const Float2& offset, const Float3& color, float spacing = 0);

			int AddRightTop(const std::string& text, float height, const Float2& offset, const Float3& color, float spacing = 0);
			int AddRightCenter(const std::string& text, float height, const Float2& offset, const Float3& color, float spacing = 0);
			int AddRightBot(const std::string& text, float height, const Float2& offset, const Float3& color, float spacing = 0);


		private:
			using Vertex = Rendering::V_Pos2Uv;
			using Instance = Rendering::I_Pos2UvCol;
			struct Entry
			{
				bool Active{ false };
				List<int> Characters{};

				static bool IsValid(const Entry& entry) { return entry.Active; }
				static void Invalidate(Entry& entry) { entry.Active = false; }
			};

			Rendering::NdcRectRenderer<Vertex, Instance> m_Renderer;
			Rendering::Texture m_FontAtlas;
			Rendering::DepthStencilState m_DepthStencilState;
			Rendering::SamplerState m_SamplerState;
			Rendering::BlendState m_BlendState;

			Rendering::FontAtlasLookup m_CharLookup;
			float m_UvXHeight;
			float m_UvHeightToWidth;

			InvalidateList<Entry> m_Entries;

			int Add(char c, const Float2& pivot, const Float2& offset, const Float3& color, const Float2& screenSize, const RectFloat& uvRect);
		};
	}
}
