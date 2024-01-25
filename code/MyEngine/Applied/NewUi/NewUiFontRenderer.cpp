#include "pch.h"
#include "NewUiFontRenderer.h"

#include "App/ResizedEvent.h"
#include "DataStructures/Adders/ListAdder.h"
#include "Framework/Resources.h"
#include "Rendering/Font/FontAtlas.h"

using namespace NewUi;
using namespace Rendering;

NewUiFontRenderer::NewUiFontRenderer(const Float2& screenSize)
	: m_InputLayout{ InputLayout::FromType<Vertex>() }
	, m_Shader{ Resources::GlobalShader(L"Font.hlsl") }
	, m_RasterizerState{ false }
	, m_NrVertices{}
	, m_ScreenSpaceToNdc{ CalculateScreenSpaceToNdc(screenSize) }
	, m_HalfScreenSize{ screenSize / 2.f }
{
	Font::FontAtlas fontAtlas{ 64 };
	m_FontAtlas = Texture{ fontAtlas.GetImage() };
	m_Assembler = TextAssembler{ std::move(fontAtlas) };
}

void NewUiFontRenderer::OnCanvasResized(const App::ResizedEvent& event)
{
	m_ScreenSpaceToNdc = CalculateScreenSpaceToNdc(event.NewSize);
	m_HalfScreenSize = Float2{ event.NewSize } / 2.f;
}

void NewUiFontRenderer::Update()
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

void NewUiFontRenderer::Render()
{
	Update();
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

unsigned NewUiFontRenderer::Add(const TextInfo& text, const Float2& position)
{
	Entry* pEntry;
	const unsigned id{ m_Entries.Validate(pEntry) };

	m_Assembler.AssembleInto([&text](const Float2& pos, const Float2& uv)
		{
			return Vertex{ pos, text.Color, uv };
		}, ListAdder<Vertex>{pEntry->Vertices}, (position - m_HalfScreenSize)* m_ScreenSpaceToNdc, { 0,0 }, text.Text, m_ScreenSpaceToNdc* text.Scale);

	m_NrVertices += pEntry->Vertices.GetSize();
	return id;
}

unsigned NewUiFontRenderer::Add_XCenter(const TextInfo& text, const Float2& position)
{
	Entry* pEntry;
	const unsigned id{ m_Entries.Validate(pEntry) };

	m_Assembler.AssembleInto_XCenter([&text](const Float2& pos, const Float2& uv)
		{
			return Vertex{ pos, text.Color, uv };
		}, ListAdder<Vertex>{pEntry->Vertices}, (position - m_HalfScreenSize)* m_ScreenSpaceToNdc, { 0,0 }, text.Text, m_ScreenSpaceToNdc* text.Scale);

	m_NrVertices += pEntry->Vertices.GetSize();
	return id;
}

void NewUiFontRenderer::Remove(unsigned id)
{
	m_NrVertices -= static_cast<const InvalidateList<Entry>&>(m_Entries).Get(id).Vertices.GetSize();
	m_Entries.Invalidate(id);
}

void NewUiFontRenderer::Clear()
{
	m_Entries.Clear();
	m_NrVertices = 0;
}

float NewUiFontRenderer::GetMaxTextHeight(float scale) const
{
	return m_Assembler.GetMaxTextHeight(scale);
}

Float2 NewUiFontRenderer::GetTextSize(const std::string& text, float scale)
{
	float baseline;
	return m_Assembler.GetSize(text, scale, baseline);
}

Float2 NewUiFontRenderer::GetTextSize_XCenter(const std::string& text, float scale)
{
	float baseline;
	return m_Assembler.GetSize_XCenter(text, scale, baseline);
}

void NewUiFontRenderer::WriteVertices(Vertex* pTarget) const
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

Float2 NewUiFontRenderer::CalculateScreenSpaceToNdc(const Float2& screenSpace)
{
	return Float2{ 1.f / screenSpace.x, 1.f / screenSpace.y }*2.f;
}
