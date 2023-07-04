#include "pch.h"
#include "FontRenderer.h"

#include "Framework/Resources.h"
#include "Image.h"
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
}

void Gui::FontRenderer::OnCanvasResized(const App::ResizedEvent& event)
{
	m_Renderer.OnCanvasResized(event);
}

void Gui::FontRenderer::Render()
{
	m_BlendState.Activate();
	m_DepthStencilState.Activate();
	m_SamplerState.Activate();
	m_FontAtlas.Activate();

	m_Renderer.Render();
}

void Gui::FontRenderer::Remove(int id)
{
	m_Entries.Invalidate(id);
	const Entry& entry{ m_Entries.GetData()[id] };

	for (int i = 0; i < entry.Characters.GetSize(); i++)
		m_Renderer.Remove(entry.Characters[i]);
}

int Gui::FontRenderer::Add(char c, const Float2& pivot, const Float2& offset, const Float3& color,
	const Float2& screenSize, const RectFloat& uvRect)
{
	const int idx{ m_Renderer.Add(pivot, offset, screenSize) };
	Instance& instance{ m_Renderer.Get(idx) };
	instance.color = color;
	instance.uvOffset = uvRect.GetLeftBot();
	instance.uvScale = uvRect.GetSize();
	return idx;
}

int Gui::FontRenderer::Add(const std::string& text, const Float2& pivot, const Float2& offset, float height,
	const Float3& color, float spacing)
{
	//retrieve existing entry struct (so an existing list can be reused)
	Entry* pEntry;
	const int idx{ m_Entries.Validate(pEntry) };
	pEntry->Active = true;
	pEntry->Characters.Clear();

	//calculate the total text size (used for alignment)
	const Float2 totalSize{ m_CharLookup.GetScreenSize(text, height) };

	//add each character
	const Float2 normalizedPivot{ (pivot + Float2{1,1}) / 2 };
	float currentX{ offset.x - totalSize.x * normalizedPivot.x };
	for (const char c : text)
	{
		if (c == ' ')
		{
			currentX += m_CharLookup.GetScreenSpaceWidth(height);
			continue;
		}
		//calculate & add characters rect
		RectFloat charUvRect;
		Float2 charScreenSize;
		m_CharLookup.Lookup(c, height, charUvRect, charScreenSize);

		const float charYOffset{ (charScreenSize.y - totalSize.y) * normalizedPivot.y };

		currentX += charScreenSize.x * normalizedPivot.x;
		pEntry->Characters.Add(Add(c, pivot, { currentX, offset.y + charYOffset }, color, charScreenSize, charUvRect));
		currentX += charScreenSize.x * (1 - normalizedPivot.x);
	}
	return idx;
}

int Gui::FontRenderer::AddLeftTop(const std::string& text, float height, const Float2& offset, const Float3& color,
	float spacing)
{
	return Add(text, { -1,1 }, offset, height, color, spacing);
}

int Gui::FontRenderer::AddLeftCenter(const std::string& text, float height, const Float2& offset, const Float3& color,
	float spacing)
{
	return Add(text, { -1, 0 }, offset, height, color, spacing);
}

int Gui::FontRenderer::AddLeftBot(const std::string& text, float height, const Float2& offset, const Float3& color,
	float spacing)
{
	return Add(text, { -1,-1 }, offset, height, color, spacing);
}

int Gui::FontRenderer::AddCenterTop(const std::string& text, float height, const Float2& offset, const Float3& color,
	float spacing)
{
	return Add(text, { 0, 1 }, offset, height, color, spacing);
}

int Gui::FontRenderer::AddCenter(const std::string& text, float height, const Float2& offset, const Float3& color,
	float spacing)
{
	return Add(text, { 0,0 }, offset, height, color, spacing);
}

int Gui::FontRenderer::AddCenterBot(const std::string& text, float height, const Float2& offset, const Float3& color,
	float spacing)
{
	return Add(text, { 0, -1 }, offset, height, color, spacing);
}

int Gui::FontRenderer::AddRightTop(const std::string& text, float height, const Float2& offset, const Float3& color,
	float spacing)
{
	return Add(text, { 1, 1 }, offset, height, color, spacing);
}

int Gui::FontRenderer::AddRightCenter(const std::string& text, float height, const Float2& offset, const Float3& color,
	float spacing)
{
	return Add(text, { 1, 0 }, offset, height, color, spacing);
}

int Gui::FontRenderer::AddRightBot(const std::string& text, float height, const Float2& offset, const Float3& color,
	float spacing)
{
	return Add(text, { 1, -1 }, offset, height, color, spacing);
}

Float2 Gui::FontRenderer::GetSize(const std::string& text, float height, float spacing)
{
	return m_CharLookup.GetScreenSize(text, height);
}

void Gui::FontRenderer::SetColor(int id, const Float3& color)
{
	const Entry& entry{ m_Entries.Get(id) };
	for (int i = 0; i < entry.Characters.GetSize(); i++)
		m_Renderer.Get(entry.Characters[i]).color = color;
}

Float2 Gui::FontRenderer::GetBiggestSize(const std::string* pText, unsigned count, float height, float spacing)
{
	Float2 biggest{};
	for (unsigned i = 0; i < count; i++)
	{
		const Float2 size{ GetSize(pText[i], height, spacing) };
		if (size.x > biggest.x) biggest.x = size.x;
		if (size.y > biggest.y) biggest.y = size.y;
	}
	return biggest;
}
