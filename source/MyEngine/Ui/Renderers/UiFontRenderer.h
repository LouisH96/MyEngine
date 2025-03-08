#pragma once
#include "DataStructures/InvalidateList.h"
#include "Font/TextAssembler.h"
#include "Rendering/Buffers/Buffer.h"

namespace MyEngine
{
namespace Ui
{
/*
	Class that (should be only) responsible 
		for the graphical part of text in the UI.

	Right now this gets also used to request the size a text will have.
		Todo: remove this? I Think this info should be requested
			1. through the Element (for the user) or
			2. through the TextAssembler (for an Element that needs it internally)
				that gets returned in this class.
*/

class UiFontRenderer
{
public:
	struct TextInfo
	{
		Float3 Color{};
		Float2 Position{};
		Float2 Pivot{};
		std::string Text{};
		float Scale{ 1 };
	};

	UiFontRenderer(const Float2& screenSize);

	void OnCanvasResized(const App::ResizedEvent& event);
	void Render();

	unsigned Add(const TextInfo& text);
	void Remove(unsigned id);
	void Clear();

	float GetMaxTextHeight(float scale) const;
	Float2 GetTextSize(const std::string& text, float scale);
	Float2 GetTextSize(const std::string& text, float scale, float& baseline);

	void EditColor(unsigned id, const Float3& newColor);
	void EditText(unsigned id, const std::string& newText);

	const Float3& GetColor(unsigned id) const;

	const TextAssembler& GetTextAssembler() const { return m_Assembler; }

private:
	using Vertex = Rendering::V_Pos2ColUv;
	struct Entry
	{
		List<Vertex> Vertices;
		Float2 NdcPosition{ -Float::MAX, 0 };
		Float2 Pivot;
		Float2 NdcScale;
		Float3 Color;

		bool IsValid() const { return NdcPosition.x != -Float::MAX; }
		void Invalidate() { NdcPosition.x = -Float::MAX; }

		/*
			Not using Vertices.Size anymore for IsValid(),
				because it doesn't work when the text is all spaces.
		*/
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
	Float2 PositionToNdc(const Float2& position) const;
	Float2 SizeToNdc(const Float2& size) const;
	Float2 SizeToNdc(float size) const;
	static Float2 CalculateScreenSpaceToNdc(const Float2& screenSpace);
};
}
}
