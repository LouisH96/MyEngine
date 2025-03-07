#include "pch.h"
#include "UiFontRenderer.h"

#include <App/ResizedEvent.h>
#include <DataStructures/Adders/ListAdder.h>
#include <Framework/Resources.h>
#include <Rendering/Font/FontAtlas.h>

#define MY_DEBUG

using namespace Ui;
using namespace Rendering;

UiFontRenderer::UiFontRenderer(const Float2& screenSize)
	: m_InputLayout{ InputLayout::FromType<Vertex>() }
	, m_Shader{ Resources::GlobalShader(L"Font.hlsl") }
	, m_RasterizerState{ false }
	, m_NrVertices{}
	, m_ScreenSpaceToNdc{ CalculateScreenSpaceToNdc(screenSize) }
	, m_HalfScreenSize{ screenSize / 2.f }
{
	Font::FontAtlas fontAtlas{ 64, Resources::Global(LR"(Fonts/Cascadia.ttf)") };
	m_FontAtlas = Texture{ fontAtlas.GetImage() };
	m_Assembler = TextAssembler{ std::move(fontAtlas) };
}

void UiFontRenderer::OnCanvasResized(const App::ResizedEvent& event)
{
	m_ScreenSpaceToNdc = CalculateScreenSpaceToNdc(event.NewSize);
	m_HalfScreenSize = Float2{ event.NewSize } / 2.f;
}

void UiFontRenderer::Update()
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

void UiFontRenderer::Render()
{
	Update();
	PrimitiveTopology::Activate(TOPOLOGY);
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

unsigned UiFontRenderer::Add(const TextInfo& text)
{
	Entry* pEntry;
	const unsigned id{ m_Entries.Validate(pEntry) };
	pEntry->Pivot = text.Pivot;

	pEntry->NdcPosition = PositionToNdc(text.Position);
	pEntry->NdcScale = SizeToNdc(text.Scale);

	m_Assembler.AssembleInto_XCenter([&text](const Float2& pos, const Float2& uv)
		{
			return Vertex{ pos, text.Color, uv };
		}, ListAdder<Vertex>{pEntry->Vertices}, pEntry->NdcPosition, pEntry->Pivot, text.Text, pEntry->NdcScale);

	m_NrVertices += pEntry->Vertices.GetSize();
	return id;
}

void UiFontRenderer::Remove(unsigned id)
{
	const List<Vertex>& vertices{ m_Entries.Get(id).Vertices };
	m_NrVertices -= vertices.GetSize();
	m_Entries.Invalidate(id);
}

void UiFontRenderer::Clear()
{
	m_Entries.Clear();
	m_NrVertices = 0;
}

float UiFontRenderer::GetMaxTextHeight(float scale) const
{
	return m_Assembler.GetMaxTextHeight(scale);
}

Float2 UiFontRenderer::GetTextSize(const std::string& text, float scale)
{
	float baseline;
	return m_Assembler.GetSize_XCenter(text, scale, baseline);
}

Float2 UiFontRenderer::GetTextSize(const std::string& text, float scale, float& baseline)
{
	return m_Assembler.GetSize_XCenter(text, scale, baseline);
}

void UiFontRenderer::EditColor(unsigned id, const Float3& newColor)
{
	Entry& entry{ m_Entries.Get(id) };

	for (unsigned i = 0; i < entry.Vertices.GetSize(); i++)
		entry.Vertices[i].col = newColor;
}

void UiFontRenderer::EditText(unsigned id, const std::string& newText)
{
	Entry& entry{ m_Entries.Get(id) };

#ifdef MY_DEBUG
	if (newText.size() * VERTICES_PER_CHAR != entry.Vertices.GetSize())
		Logger::Warning("[UiFontRenderer::EditText] newText is different length");
#endif

	const Float3 color{ entry.Vertices.First().col };
	entry.Vertices.Clear();

	m_Assembler.AssembleInto_XCenter([&color](const Float2& pos, const Float2& uv)
		{
			return Vertex{ pos, color, uv };
		}, ListAdder<Vertex>{entry.Vertices}, entry.NdcPosition, entry.Pivot, newText, entry.NdcScale);
}

const Float3& UiFontRenderer::GetColor(unsigned id) const
{
	return m_Entries.Get(id).Vertices.First().col;
}

void UiFontRenderer::WriteVertices(Vertex* pTarget) const
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

Float2 UiFontRenderer::CalculateScreenSpaceToNdc(const Float2& screenSpace)
{
	return Float2{ 1.f / screenSpace.x, 1.f / screenSpace.y }*2.f;
}

Float2 UiFontRenderer::PositionToNdc(const Float2& position) const
{
	return (position - m_HalfScreenSize) * m_ScreenSpaceToNdc;
}

Float2 UiFontRenderer::SizeToNdc(const Float2& size) const
{
	return m_ScreenSpaceToNdc * size;
}

Float2 UiFontRenderer::SizeToNdc(float size) const
{
	return m_ScreenSpaceToNdc * size;
}

#undef MY_DEBUG