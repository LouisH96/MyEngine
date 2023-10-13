#include "pch.h"
#include "FontAtlasLookup.h"

#include "FontAtlas.h"
#include "Image/Image.h"

Rendering::FontAtlasLookup::FontAtlasLookup(Font::FontAtlas&& fontAtlas)
	: m_Positions{ std::move(fontAtlas.GetCharacterHorPos()) }
	, m_Heights{ std::move(fontAtlas.GetCharacterHeight()) }
	, m_BaselineOffset{ std::move(fontAtlas.GetBaselineOffset()) }
{
	m_SpaceWidthRatio = fontAtlas.GetSpaceWidth() / GetCharUvWidth('x');
	m_InvXUvHeight = 1.f / GetCharUvHeight('x');
	m_UvWidthToHeight = Float::Cast(fontAtlas.GetImage().GetWidth()) / Float::Cast(fontAtlas.GetImage().GetHeight());
}

void Rendering::FontAtlasLookup::Lookup(char c, float xPixelHeight, RectFloat& uvRect, Float2& screenSize)
{
	uvRect = GetCharUvRect(c);
	screenSize.y = uvRect.GetHeight() * m_InvXUvHeight * xPixelHeight;
	screenSize.x = uvRect.GetWidth() * m_UvWidthToHeight * screenSize.y / uvRect.GetHeight();
}

float Rendering::FontAtlasLookup::GetCharUvWidth(char c)
{
	const int idx{ c };
	return m_Positions[idx + 1] - m_Positions[idx];
}

float Rendering::FontAtlasLookup::GetCharUvHeight(char c)
{
	return m_Heights[c];
}

Float2 Rendering::FontAtlasLookup::GetCharUvSize(char c)
{
	const int idx{ c };
	return{
		m_Positions[idx + 1] - m_Positions[idx],
		m_Heights[idx] };
}

RectFloat Rendering::FontAtlasLookup::GetCharUvRect(char c)
{
	const int idx{ c };
	RectFloat rect{ {m_Positions[idx], m_Heights[idx]}, {m_Positions[idx + 1], m_Heights[idx]} };
	rect.SetWidth(rect.GetWidth() - rect.GetLeft());
	return rect;
}

float Rendering::FontAtlasLookup::GetUvWidth(const std::string& text, float uvSpacingWidth)
{
	float width{ 0 };
	for (const char& c : text)
	{
		if (c == ' ') width += GetCharUvWidth('x') * m_SpaceWidthRatio;
		else width += GetCharUvWidth(c) + uvSpacingWidth;
	}
	if (text[text.size() - 1] != ' ')
		width -= uvSpacingWidth;
	return width;
}

Float2 Rendering::FontAtlasLookup::GetUvSize(const std::string& text, float uvSpacingWidth)
{
	float lowest{ Float::Max() };
	float highest{ -Float::Max() };

	float totalWidth{ 0 };
	for (const char& c : text)
	{
		if (c == ' ')
		{
			totalWidth += GetCharUvWidth('x') * m_SpaceWidthRatio;
			continue;
		}
		const Float2 charSize{ GetCharUvSize(c) };
		totalWidth += charSize.x + uvSpacingWidth;

		const float lower{ m_BaselineOffset[c] };
		const float upper{ lower + charSize.y };

		if (lower < lowest) lowest = lower;
		if (upper > highest)highest = upper;

	}
	if (text[text.size() - 1] != ' ')
		totalWidth -= uvSpacingWidth;
	return { totalWidth, highest - lowest };
}

float Rendering::FontAtlasLookup::GetScreenWidth(const std::string& text, float height, float spacing)
{
	const float xUvWidth{ GetCharUvWidth('x') };
	const float xScreenWidth{ xUvWidth * m_UvWidthToHeight * m_InvXUvHeight * height };
	const float uvSpacing{ spacing / xScreenWidth * xUvWidth };
	const float textUvWidth{ GetUvWidth(text, uvSpacing) };
	return  textUvWidth / xUvWidth * xScreenWidth;
}

float Rendering::FontAtlasLookup::GetScreenWidth(char c, float height)
{
	const float xUvWidth{ GetCharUvWidth('x') };
	const float xScreenWidth{ xUvWidth * m_UvWidthToHeight * m_InvXUvHeight * height };
	const float textUvWidth{ GetCharUvWidth(c) };
	return  textUvWidth / xUvWidth * xScreenWidth;
}

float Rendering::FontAtlasLookup::GetScreenSpaceWidth(float xHeight)
{
	const float xUvWidth{ GetCharUvWidth('x') };
	const float xScreenWidth{ xUvWidth * m_UvWidthToHeight * m_InvXUvHeight * xHeight };
	return xScreenWidth * m_SpaceWidthRatio;
}

Float2 Rendering::FontAtlasLookup::GetScreenSize(const std::string& text, float height, float spacing)
{
	const float xUvWidth{ GetCharUvWidth('x') };
	const float xScreenWidth{ xUvWidth * m_UvWidthToHeight * m_InvXUvHeight * height };
	const float uvSpacing{ spacing / xScreenWidth * xUvWidth };
	const Float2 uvSize{ GetUvSize(text, uvSpacing) };
	Float2 screenSize;
	screenSize.y = uvSize.y * m_InvXUvHeight * height;
	screenSize.x = screenSize.y * uvSize.x * m_UvWidthToHeight / uvSize.y;
	return screenSize;
}

Float2 Rendering::FontAtlasLookup::GetScreenSize(char c, float height)
{
	const Float2 uvSize{ GetCharUvSize(c) };
	Float2 screenSize;
	screenSize.y = uvSize.y * m_InvXUvHeight * height;
	screenSize.x = screenSize.y * uvSize.x * m_UvWidthToHeight / uvSize.y;
	return screenSize;
}

float Rendering::FontAtlasLookup::GetBaselineOffsetUvSize(char c)
{
	return m_BaselineOffset[c];
}

float Rendering::FontAtlasLookup::GetBaselineOffsetScreenSize(char c, float height)
{
	return m_BaselineOffset[c] * m_InvXUvHeight * height;
}
