#pragma once
#include "DataStructures/InvalidateList.h"
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
			struct TextInfo
			{
				std::string Text;
				float Scale;
				Float3 Color;
			};
			struct PositionInfo
			{
				Float2 Position;
				Float2 Pivot;

			};
			static constexpr float HEADER_FONT_SIZE{ .2f };

			NodeGraphFontRenderer();

			void Update();
			void Render();

			int Add(const TextInfo& text, const PositionInfo& position);
			int Add(const TextInfo& text, const PositionInfo& position, const Float2& textSize, float baseline);
			void Remove(int id);

			void Move(unsigned id, const Float2& amount);

			float GetMaxTextHeight() const;
			Float2 GetTextSize(const std::string& text, float scale, float& baseline) const;

		private:
			struct Entry
			{
				List<Vertex> Vertices;

				bool IsValid() const { return Vertices.Any(); }
				void Invalidate() { Vertices.Clear(); }
			};

			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;

			Rendering::Texture m_FontAtlas;
			Rendering::DepthStencilState m_DepthStencilState;
			Rendering::SamplerState m_SamplerState;
			Rendering::BlendState m_BlendState;
			Rendering::RasterizerState m_RasterizerState;

			Rendering::Buffer<Vertex> m_Vertices;

			InvalidateList<Entry> m_Entries;
			unsigned m_NrVertices;

			TextAssembler m_Assembler;

			void WriteVertices(Vertex* pTarget) const;
		};
	}
}
