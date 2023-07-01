#include "pch.h"
#include "FontRenderer.h"

#include "Framework/Resources.h"
#include "Rendering/Image.h"
#include "Rendering/Font/FontAtlas.h"

using namespace Rendering;

Gui::FontRenderer::FontRenderer()
	: m_Renderer{ Resources::GlobalShader(L"Font_Inst.hlsl") }
	, m_DepthStencilState{ false }
{
	//ATLAS
	Font::FontAtlas fontAtlas{64};
	m_FontAtlas = Texture{ fontAtlas.GetImage() };
	m_CharLookup = FontAtlasLookup{ std::move(fontAtlas) };

	m_UvXHeight = m_CharLookup.GetCharUvHeight('x');
	m_UvHeightToWidth = Float::Cast(fontAtlas.GetImage().GetHeight()) / Float::Cast(fontAtlas.GetImage().GetWidth());

	Add('x', { 0,-1 }, { 0,50 }, 75, { 1,0,0 });
}

void Gui::FontRenderer::OnCanvasResized(const App::ResizedEvent& event)
{
	m_Renderer.OnCanvasResized(event);
}

void Gui::FontRenderer::Render()
{
	m_DepthStencilState.Activate();
	m_SamplerState.Activate();
	m_FontAtlas.Activate();

	m_Renderer.Render();
}

void Gui::FontRenderer::Remove(int id)
{
	m_Renderer.Remove(id);
}

int Gui::FontRenderer::Add(char c, const Float2& pivot, const Float2& offset, float height, const Float3& color)
{
	RectFloat uvRect{};
	Float2 screenSize{};
	m_CharLookup.Lookup(c, height, uvRect, screenSize);

	const int idx{ m_Renderer.Add(pivot, offset, screenSize) };
	Instance& instance{ m_Renderer.Get(idx) };
	instance.color = color;
	instance.uvOffset = uvRect.GetLeftBot();
	instance.uvScale = uvRect.GetSize();
	return idx;
}
