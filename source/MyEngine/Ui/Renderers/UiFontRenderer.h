#pragma once
#include "DataStructures/InvalidateList.h"
#include "Font/TextAssembler.h"
#include "Rendering/Buffers/Buffer.h"

namespace MyEngine
{
namespace Ui
{
class UiFontRenderer
{
public:
	struct TextInfo
	{
		std::string Text;
		float Scale;
		Float3 Color;
	};

	UiFontRenderer(const Float2& screenSize);

	void OnCanvasResized(const App::ResizedEvent& event);
	void Render();

	unsigned Add(const TextInfo& text, const Float2& position);

	//x-center mode by default
	unsigned Add(const TextInfo& text, const Float2& position, const Float2& pivot);
	unsigned Add_XCenter(const TextInfo& text, const Float2& position);
	void Remove(unsigned id);
	void Clear();

	float GetMaxTextHeight(float scale) const;
	Float2 GetTextSize(const std::string& text, float scale);
	Float2 GetTextSize_XCenter(const std::string& text, float scale);

	void EditColor(unsigned id, const Float3& newColor);

private:
	using Vertex = Rendering::V_Pos2ColUv;
	struct Entry
	{
		List<Vertex> Vertices;

		bool IsValid() const { return Vertices.Any(); }
		void Invalidate() { Vertices.Clear(); }
	};
	static constexpr ModelTopology TOPOLOGY{ ModelTopology::TriangleList };
	static constexpr unsigned VERTICES_PER_CHAR{ 6 };

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

	Float2 m_ScreenSpaceToNdc;
	Float2 m_HalfScreenSize;

	void Update();
	void WriteVertices(Vertex* pTarget) const;
	static Float2 CalculateScreenSpaceToNdc(const Float2& screenSpace);
};
}
}
