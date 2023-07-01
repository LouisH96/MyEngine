#include "pch.h"
#include "FontAtlasLookup.h"

#include "FontAtlas.h"
#include "Rendering/Image.h"

Rendering::FontAtlasLookup::FontAtlasLookup(Font::FontAtlas&& fontAtlas)
	: m_Positions{ std::move(fontAtlas.GetCharacterHorPos()) }
	, m_Heights{ std::move(fontAtlas.GetCharacterHeight()) }
	, m_SpaceWidth{ fontAtlas.GetSpaceWidth() }
{
	m_ToXRatio = 1.f / GetCharUvHeight('x');
	m_UvWidthToHeight = Float::Cast(fontAtlas.GetImage().GetWidth()) / Float::Cast(fontAtlas.GetImage().GetHeight());
}

void Rendering::FontAtlasLookup::Lookup(char c, float xPixelHeight, RectFloat& uvRect, Float2& screenSize)
{
	uvRect = GetCharUvRect(c);
	screenSize.y = uvRect.GetHeight() * m_ToXRatio * xPixelHeight;
	screenSize.x = uvRect.GetWidth() * m_UvWidthToHeight * screenSize.y / uvRect.GetHeight();
}

float Rendering::FontAtlasLookup::GetCharUvWidth(char c)
{
	const int idx{ CharToIdx(c) };
	return m_Positions[idx + 1] - m_Positions[idx];
}

float Rendering::FontAtlasLookup::GetCharUvHeight(char c)
{
	return m_Heights[CharToIdx(c)];
}

Float2 Rendering::FontAtlasLookup::GetCharUvSize(char c)
{
	const int idx{ CharToIdx(c) };
	return{
		m_Positions[idx + 1] - m_Positions[idx],
		m_Heights[idx] };
}

RectFloat Rendering::FontAtlasLookup::GetCharUvRect(char c)
{
	const int idx{ CharToIdx(c) };
	RectFloat rect{ {m_Positions[idx], m_Heights[idx]}, {m_Positions[idx + 1], m_Heights[idx]} };
	rect.SetWidth(rect.GetWidth() - rect.GetLeft());
	return rect;
}

int Rendering::FontAtlasLookup::CharToIdx(char c)
{
	if (c >= 'a') return c - 'a';
	if (c >= 'A') return 'z' - 'a' + 1 + c - 'A';
	return ('z' - 'a' + 1) * 2 + c - '0';
}
