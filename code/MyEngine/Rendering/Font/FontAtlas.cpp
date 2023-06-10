#include "pch.h"
#include "FontAtlas.h"

#include <fstream>

#include "Framework/Resources.h"
#include "Io/Ttf/FontRasterizer.h"
#include "Io/Ttf/TtfReader.h"
#include "Rendering/Image.h"

using namespace Io::Ttf;

Rendering::Font::FontAtlas::FontAtlas()
{
	//read
	const std::wstring fontPath = Framework::Resources::GetGlobalResourcePath(L"Fonts\\ShortBaby.ttf");
	std::ifstream stream{ fontPath, std::ios::binary };
	const Io::TtfReader reader{ stream };

	//create empty atlas
	constexpr float desiredXWidth{ .1f };
	constexpr int pixelsXWidth{ 32 };
	const float ttfToPixels{ pixelsXWidth / static_cast<float>(reader.GetGlyph('x').GetSize().x) };

	const Int2 atlasSize{ GetBounds(reader, ttfToPixels) };
	m_pImage = new Image{ atlasSize };

	m_WorldSize = Float2(atlasSize) * desiredXWidth / pixelsXWidth;

	//draw atlas
	int drawX{ 0 };
	for (char lower = 'a'; lower <= 'z'; lower++) DrawAtlasStep(reader, *m_pImage, drawX, ttfToPixels, lower);
	for (char upper = 'A'; upper <= 'Z'; upper++)  DrawAtlasStep(reader, *m_pImage, drawX, ttfToPixels, upper);
	for (char number = '0'; number <= '9'; number++)  DrawAtlasStep(reader, *m_pImage, drawX, ttfToPixels, number);
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

Float2 Rendering::Font::FontAtlas::GetBounds(const Io::TtfReader& reader, float ttfToPixels)
{
	Int2 bounds{ 0, 0 };
	for (char lower = 'a'; lower <= 'z'; lower++) DoBoundsStep(reader, bounds, ttfToPixels, lower);
	for (char upper = 'A'; upper <= 'Z'; upper++) DoBoundsStep(reader, bounds, ttfToPixels, upper);
	for (char number = '0'; number <= '9'; number++) DoBoundsStep(reader, bounds, ttfToPixels, number);
	return bounds;
}

void Rendering::Font::FontAtlas::DoBoundsStep(const Io::TtfReader& reader, Int2& bounds, float ttfToPixels, char character)
{
	const Glyph glyph{ reader.GetGlyph(character) };
	const Int2 sizeInPixels{ (glyph.GetSize() * ttfToPixels).Ceiled() };
	bounds.x += sizeInPixels.x;
	if (sizeInPixels.y > bounds.y) bounds.y = sizeInPixels.y;
}

void Rendering::Font::FontAtlas::DrawAtlasStep(const Io::TtfReader& reader, Image& atlasImage, int& x, float ttfToPixels, char character)
{
	const Glyph glyph{ reader.GetGlyph(character) };
	const Int2 sizeInPixels{ (glyph.GetSize() * ttfToPixels).Ceiled() };

	const FontRasterizer rasterizer{ glyph, sizeInPixels.x,sizeInPixels.y };
	const Image* pGlyphImage{ rasterizer.MakeImage({}) };

	pGlyphImage->CopyTo(atlasImage, { x, 0 });
	x += sizeInPixels.x;

	delete pGlyphImage;
}
