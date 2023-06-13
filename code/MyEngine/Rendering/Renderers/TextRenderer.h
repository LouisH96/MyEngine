#pragma once
#include <Rendering/Renderers/RendererFactory.h>

#include "Math/Rects.h"
#include "Rendering/DrawData/InstanceList.h"
#include "Rendering/DrawData/VertexList.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class FontRasterizer;
		}
	}
	namespace Game
	{
		class FocusPointCamera;
	}

	namespace Rendering
	{
		class Gpu;

		class TextRenderer
		{
		public:
			//---| Types |---
			class TextRendererElementId
			{
			public:
				TextRendererElementId() = default;
				explicit TextRendererElementId(unsigned id);
				int GetId() const { return m_Id; }
				bool IsValid() const { return m_Id >= 0; }
				bool operator==(const TextRendererElementId& other) const;
			private:
				int m_Id;
			};
			TextRenderer();
			~TextRenderer() = default;

			void OnCanvasResize(const Int2& newSize);
			void Render();

			TextRendererElementId AddLeftBottom(const Int2& leftBot, float xWidth, const std::string& text, const Float3& color = {}, float spacing = 0);
			TextRendererElementId AddCenterBottom(const Int2& leftBot, float xWidth, const std::string& text, const Float3& color = {}, float spacing = 0);

			float GetScreenWidth(float xWidth, const std::string& text, float spacing = 0) const;

		private:
			using Vertex = V_Pos2Uv;
			struct Instance
			{
				Float2 PositionOffset;
				Float2 Size;
				Float2 UvScale;
				Float2 UvOffset;
				Float3 Color;
			};
			static constexpr int VERTICES_PER_RECT = 6;
			static const InputLayout::Element ELEMENTS[];

			BlendState m_BlendState;
			InputLayout m_InputLayout;
			RasterizerState m_RasterizerState;
			SamplerState m_SamplerState;
			Shader m_Shader;
			DepthStencilState m_DepthStencilState;

			Array<float> m_CharactersHorPos;
			Array<float> m_CharactersHeight;
			float m_SpaceWidth;

			Texture m_FontAtlas;
			float m_HeightToWidth; //in uv
			float m_UvToScreen; //multiply uv-width with this to be in screen-width (1/'x'-uv-width), result is 'x' * this would be 1 pixel screen-space

			Int2 m_CanvasSize;
			InstanceList<Vertex, Instance> m_Instances;
			unsigned m_CenterBottomAnchoredIdx; //begin of vertices anchored to center-bottom (before is anchored to left-bottom)

			void Add(int idx, const RectFloat& rect, const RectFloat& uvRect, const Float3& color);
			void Replace(int idx, const RectFloat& rect);
			int ToVertexIdx(TextRendererElementId id) const;

			float GetCharUvWidth(int charIdx) const;
			float GetCharUvWidth(char c) const;
			Float2 GetCharUvSize(int charIdx) const;
			Float2 GetCharUvSize(char c) const;
			RectFloat GetCharUvRect(char c) const;
			RectFloat GetCharUvRect(int charIdx) const;
			//toclip functions should be renamed to toNdc
			static float ToClipAlignMin(int screenPos, float screenSize);
			static float ToClipAlignCenter(int screenPos, float screenSize);
			static float SizeToClip(float size, float screenSize);
			static float SizeToClip(int size, float screenSize);
			static Float2 SizeToClip(const Float2& size, const Float2& screenSize);
			static Float2 SizeToClip(const Int2& size, const Float2& screenSize);
			static int CharacterToIndex(char c);
		};
	}
}
