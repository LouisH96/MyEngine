#include "pch.h"
#include "FontAtlas.h"

#include <fstream>

#include "Framework/Resources.h"
#include "Io/Ttf/FontRasterizer.h"
#include "Io/Ttf/TtfReader.h"
#include "Rendering/Image.h"

using namespace Io::Ttf;

Rendering::Font::FontAtlas::FontAtlas(int xHorizontalPixels)
{
	//read
	const std::wstring fontPath = Framework::Resources::GetGlobalResourcePath(L"Fonts\\ShortBaby.ttf");
	std::ifstream stream{ fontPath, std::ios::binary };
	const Io::TtfReader reader{ stream };

	const float ttfToPixels{ static_cast<float>(xHorizontalPixels) / static_cast<float>(reader.GetGlyph('x').GetSize().x) };

	constexpr int nrCharacters = 'z' - 'a' + 1 + 'Z' - 'A' + 1 + '9' - '0' + 1;
	m_CharacterHorPos = { nrCharacters + 1 };
	m_CharacterHeight = { nrCharacters };
	m_CharacterHorPos[0] = 0;

	//phase1: pixel sizes
	int idx{ 0 };
	float highest{ 0 };
	for (char c = 'a'; c <= 'z'; c++, idx++) CharacterInfoStep(reader.GetGlyph(c), idx, ttfToPixels, highest);
	for (char c = 'A'; c <= 'Z'; c++, idx++) CharacterInfoStep(reader.GetGlyph(c), idx, ttfToPixels, highest);
	for (char c = '0'; c <= '9'; c++, idx++) CharacterInfoStep(reader.GetGlyph(c), idx, ttfToPixels, highest);

	//phase2: make image
	m_pImage = new Image{ static_cast<int>(m_CharacterHorPos.Last()), static_cast<int>(highest) };
	const Float2 scale{ 1 / static_cast<float>(m_pImage->GetWidth()), 1 / static_cast<float>(m_pImage->GetHeight())};
	idx = 0;
	for (char c = 'a'; c <= 'z'; c++, idx++) DrawGlyphStep(reader.GetGlyph(c), idx, ttfToPixels);
	for (char c = 'A'; c <= 'Z'; c++, idx++) DrawGlyphStep(reader.GetGlyph(c), idx, ttfToPixels);
	for (char c = '0'; c <= '9'; c++, idx++) DrawGlyphStep(reader.GetGlyph(c), idx, ttfToPixels);

	//phase3: normalize char-info
	for (int i = 0; i < m_CharacterHeight.GetSize(); i++) m_CharacterHeight[i] *= scale.y;
	for (int i = 0; i < m_CharacterHorPos.GetSize(); i++) m_CharacterHorPos[i] *= scale.x;

	//other
	m_SpaceWidth = reader.GetGlyph(' ').GetSize().x * ttfToPixels * scale.x;
}

Rendering::Font::FontAtlas::~FontAtlas()
{
	delete m_pImage;
}

Rendering::Image* Rendering::Font::FontAtlas::GetImageOwnership()
{
	Image* pReturn{ m_pImage };
	m_pImage = nullptr;
	return pReturn;
}

void Rendering::Font::FontAtlas::CharacterInfoStep(const Glyph& glyph, int idx, float ttfToPixels, float& highest)
{
	const Float2 sizeInPixels{ (glyph.GetSize() * ttfToPixels).Ceiled() };

	if (sizeInPixels.y > highest) highest = sizeInPixels.y;
	m_CharacterHeight[idx] = sizeInPixels.y;
	m_CharacterHorPos[idx + 1] = m_CharacterHorPos[idx] + sizeInPixels.x;
}

void Rendering::Font::FontAtlas::DrawGlyphStep(const Io::Ttf::Glyph& glyph, int idx, float ttfToPixels)
{
	const Int2 sizeInPixels{ (glyph.GetSize() * ttfToPixels).Ceiled() };
	const FontRasterizer rasterizer{ glyph, sizeInPixels };
	const Image* pGlyphImage{ rasterizer.MakeImage({1,1,1}) };
	pGlyphImage->CopyTo(*m_pImage, { static_cast<int>(m_CharacterHorPos[idx]), 0 });
	delete pGlyphImage;
}