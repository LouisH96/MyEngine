#include "pch.h"
#include "NodeGraphFontRenderer.h"

#include "DataStructures/Adders/ListAdder.h"
#include "Framework/Resources.h"
#include "Rendering/Font/FontAtlas.h"

using namespace Applied;
using namespace Rendering;

NodeGraphFontRenderer::NodeGraphFontRenderer()
	: m_InputLayout{ InputLayout::FromType<Vertex>() }
	, m_Shader{ Resources::GlobalShader(L"NodeGraph_Tex.hlsl") }
	, m_RasterizerState{ false }
{
	Font::FontAtlas fontAtlas{ 64 };
	m_FontAtlas = Texture{ fontAtlas.GetImage() };
	m_Assembler = TextAssembler{ std::move(fontAtlas) };
}

void NodeGraphFontRenderer::Update()
{
	if (!m_Entries.HasChanged())
		return;
	m_Entries.ClearChangedFlag();

	if (m_NrVertices > m_Vertices.GetCapacity())
	{
		const unsigned newCapacity{ m_NrVertices * 2 };
		Vertex* pVertices = new Vertex[newCapacity];
		WriteVertices(pVertices);
		m_Vertices = Buffer<Vertex>{ {pVertices, newCapacity}, true };
		delete[] pVertices;
	}
	else
	{
		Vertex* pVertices{ m_Vertices.StartCopyData() };
		WriteVertices(pVertices);
		m_Vertices.EndCopyData();
	}
}

void NodeGraphFontRenderer::Render()
{
	PrimitiveTopology::Activate(ModelTopology::TriangleList);
	m_RasterizerState.Activate();
	m_InputLayout.Activate();
	m_Shader.Activate();

	m_FontAtlas.ActivatePs();
	m_DepthStencilState.Activate();
	m_SamplerState.ActivatePs();
	m_BlendState.Activate();

	m_Vertices.Activate(0);
	m_Vertices.Draw(m_NrVertices);
}

int NodeGraphFontRenderer::Add(const TextInfo& text, const PositionInfo& position)
{
	Entry* pEntry;
	const int id{ m_Entries.Validate(pEntry) };

	m_Assembler.AssembleInto([&text](const Float2& pos, const Float2& uv)
		{
			return Vertex{ pos, text.Color, uv };
		}, ListAdder<Vertex>{pEntry->Vertices}, position.Position, position.Pivot, text.Text, Float2{ text.Scale });

	m_NrVertices += pEntry->Vertices.GetSize();
	return id;
}

int NodeGraphFontRenderer::Add(const TextInfo& text, const PositionInfo& position, const Float2& textSize,
	float baseline)
{
	Entry* pEntry;
	const int id{ m_Entries.Validate(pEntry) };

	m_Assembler.AssembleInto([&text](const Float2& pos, const Float2& uv)
		{
			return Vertex{ pos, text.Color, uv };
		}, ListAdder<Vertex>{pEntry->Vertices}, position.Position, position.Pivot, text.Text, Float2{ text.Scale },
			textSize, baseline);

	m_NrVertices += pEntry->Vertices.GetSize();
	return id;
}

void NodeGraphFontRenderer::Remove(int id)
{
	m_NrVertices -= static_cast<const InvalidateList<Entry>&>(m_Entries).Get(id).Vertices.GetSize();
	m_Entries.Invalidate(id);
}

void NodeGraphFontRenderer::Move(unsigned id, const Float2& amount)
{
	List<Vertex>& vertices{ m_Entries.Get(id).Vertices };

	for (unsigned i = 0; i < vertices.GetSize(); i++)
		vertices[i].pos += amount;
}

float NodeGraphFontRenderer::GetMaxTextHeight() const
{
	return m_Assembler.GetMaxTextHeight(HEADER_FONT_SIZE);
}

Float2 NodeGraphFontRenderer::GetTextSize(const std::string& text, float scale, float& baseline) const
{
	return m_Assembler.GetSize(text, scale, baseline);
}

void NodeGraphFontRenderer::WriteVertices(Vertex* pTarget) const
{
	const Entry* pEntriesEnd{ m_Entries.GetEnd() };

	for (const Entry* pEntry{ m_Entries.GetFirst() }; pEntry != pEntriesEnd; pEntry++)
	{
		if (!pEntry->IsValid()) continue;
		const List<Vertex>& vertices{ pEntry->Vertices };

		std::copy(vertices.GetData(), vertices.GetEnd(), pTarget);
		pTarget += vertices.GetSize();
	}
}
