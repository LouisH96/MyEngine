#include "pch.h"
#include "FontRasterizer.h"

#include "Io/Ttf/Glyph.h"
#include <Debug/DebugRenderer.h>

Io::Ttf::FontRasterizer::FontRasterizer(const Glyph& glyph)
	: m_Glyph{ glyph }
{

}

void Io::Ttf::FontRasterizer::DrawBounds(const Math::Float3& color, const Math::Float3& offset) const
{
	DebugRenderer::AddRect(m_Glyph.GetMinBounds(), m_Glyph.GetMaxBounds(), offset, color);
}

void Io::Ttf::FontRasterizer::DrawGrid(const Math::Float3& offset, const Math::Float2& bounds, int nrCols, int nrRows, const Math::Float3& color) const
{
	DebugRenderer::AddGridXy(offset, bounds, nrCols, nrRows, color);
}
