#include "pch.h"
#include "FontAtlas.h"

#include <fstream>

#include "Image/Image.h"
#include "Io/Ttf/FontRasterizer.h"
#include "Io/Ttf/TtfReader.h"

using namespace MyEngine;
using namespace Rendering::Font;
using namespace Io::Ttf;

FontAtlas::FontAtlas(int xHorizontalPixels, const std::wstring& path, const std::wstring& backupPath)
{
	//read
	std::ifstream mainStream{ path, std::ios::binary };
	Io::TtfReader mainReader{ mainStream };

	std::ifstream backupStream{ backupPath, std::ios::binary };
	Io::TtfReader backupReader{ backupStream };

	const float mainTtfToPixels{ static_cast<float>(xHorizontalPixels) / static_cast<float>(mainReader.GetGlyph('x').GetSize().x) };
	const float backupTtfToPixels{ static_cast<float>(xHorizontalPixels) / static_cast<float>(backupReader.GetGlyph('x').GetSize().x) };

	constexpr int nrCharacters = 128;
	m_CharacterHorPos = { nrCharacters + 1 };
	m_CharacterHeight = { nrCharacters };
	m_BaselineOffset = { nrCharacters };
	m_CharacterHorPos[0] = 0;

	//
	Glyph glyph{};
	float ttfToPixels;

	//phase1: pixel sizes
	float highest{ 1 };
	for (int c = 0; c < nrCharacters; c++)
	{
		GetGlyph(c, mainReader, backupReader, mainTtfToPixels, backupTtfToPixels, glyph, ttfToPixels);
		CharacterInfoStep(glyph, c, ttfToPixels, highest);
	}

	//phase2: make image
	m_pImage = new Image{ static_cast<int>(m_CharacterHorPos.Last()), static_cast<int>(highest) };
	const Float2 scale{ 1 / static_cast<float>(m_pImage->GetWidth()), 1 / static_cast<float>(m_pImage->GetHeight()) };
	for (int c = 0; c <= nrCharacters; c++)
	{
		GetGlyph(c, mainReader, backupReader, mainTtfToPixels, backupTtfToPixels, glyph, ttfToPixels);
		DrawGlyphStep(glyph, c, ttfToPixels);
	}

	//phase3: normalize char-info
	for (unsigned i = 0; i < m_CharacterHeight.GetSize(); i++) m_CharacterHeight[i] *= scale.y;
	for (unsigned i = 0; i < m_BaselineOffset.GetSize(); i++) m_BaselineOffset[i] *= scale.y;
	for (unsigned i = 0; i < m_CharacterHorPos.GetSize(); i++) m_CharacterHorPos[i] *= scale.x;

	//other
	m_SpaceWidth = static_cast<float>(mainReader.GetGlyph(' ').GetSize().x) * mainTtfToPixels * scale.x;
}

FontAtlas::~FontAtlas()
{
	delete m_pImage;
}

Rendering::Image* FontAtlas::GetImageOwnership()
{
	Image* pReturn{ m_pImage };
	m_pImage = nullptr;
	return pReturn;
}

float FontAtlas::GetImageAspectRatio() const
{
	return m_pImage->GetAspectRatio();
}

void FontAtlas::CharacterInfoStep(const Glyph& glyph, int idx, float ttfToPixels, float& highest)
{
	if (!glyph.IsValid())
	{
		m_CharacterHeight[idx] = 1;
		m_CharacterHorPos[idx + 1] = m_CharacterHorPos[idx] + 1;
		m_BaselineOffset[idx] = 0;
		return;
	}

	const Float2 sizeInPixels{ (glyph.GetSize() * ttfToPixels).Ceiled() };
	const float baselineOffset{ static_cast<float>(glyph.GetMinBounds().y * ttfToPixels) };

	if (sizeInPixels.y > highest) highest = sizeInPixels.y;
	m_CharacterHeight[idx] = sizeInPixels.y;
	m_CharacterHorPos[idx + 1] = m_CharacterHorPos[idx] + sizeInPixels.x;
	m_BaselineOffset[idx] = baselineOffset;
}

void FontAtlas::DrawGlyphStep(const Glyph& glyph, int idx, float ttfToPixels)
{
	const Int2 sizeInPixels{ (glyph.GetSize() * ttfToPixels).Ceiled() };
	if (sizeInPixels.x == 0)
		return;

	const FontRasterizer rasterizer{ glyph, sizeInPixels };
	Image* pGlyphImage{ rasterizer.MakeImage({1,1,1}) };
	pGlyphImage->CopyTo(*m_pImage, { static_cast<int>(m_CharacterHorPos[idx]), 0 });
	FontRasterizer::DeleteImage(pGlyphImage);
}

void FontAtlas::GetGlyph(int id, Io::TtfReader& mainReader, Io::TtfReader& backupReader,
	float mainTtfToPixels, float backupTtfToPixels, Io::Ttf::Glyph& glyph, float& ttfToPixels)
{
	glyph = mainReader.GetGlyph(static_cast<char>(id));
	if (glyph.IsValid())
	{
		ttfToPixels = mainTtfToPixels;
	}
	else
	{
		glyph = backupReader.GetGlyph(static_cast<char>(id));
		ttfToPixels = backupTtfToPixels;
	}
}
