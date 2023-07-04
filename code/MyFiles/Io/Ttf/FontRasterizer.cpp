#include "FontRasterizer.h"

#include "Image.h"
#include "Intersection.h"
#include "Io/Ttf/Glyph.h"
#include "Logger/Logger.h"

#undef min;
#undef max;

MyEngine::Io::Ttf::FontRasterizer::FontRasterizer(const Glyph& glyph, const Int2& sizeInPixels)
	: FontRasterizer{ glyph, sizeInPixels.x, sizeInPixels.y }
{
}

MyEngine::Io::Ttf::FontRasterizer::FontRasterizer(const Glyph& glyph, int nrCols, int nrRows)
	: m_Glyph{ glyph }
	, m_NrCols{ nrCols }
	, m_NrRows{ nrRows }
{
}

void MyEngine::Io::Ttf::FontRasterizer::DrawBounds(const Float3& color, const Float3& offset) const
{
	//todo:
	Logger::PrintError("[FontRasterizer]: DebugRenderer not in MyBasics");

	//DebugRenderer::AddRect(m_Glyph.GetMinBounds(), m_Glyph.GetMaxBounds(), offset, color);
}

void MyEngine::Io::Ttf::FontRasterizer::DrawGrid(const Float3& offset, const Float3& color) const
{
	//todo:
	Logger::PrintError("[FontRasterizer]: DebugRenderer not in MyBasics");

	//DebugRenderer::AddGridXy(offset, m_Glyph.GetSize(), m_NrCols, m_NrRows, color);
}

void MyEngine::Io::Ttf::FontRasterizer::DrawIntersections(const Float3& offset, const Float2& bounds) const
{
	//todo:
	Logger::PrintError("[FontRasterizer]: DebugRenderer not in MyBasics");

	/*const Double2 glyphSize{ m_Glyph.GetSize() };

	std::vector<Intersection> intersections{};
	for (int iScanLine = 0; iScanLine < m_NrRows; iScanLine++)
	{
		intersections.clear();
		const double height = (static_cast<double>(iScanLine) + 0.5) / static_cast<double>(m_NrRows)
			* glyphSize.y + m_Glyph.GetMinBounds().y;
		m_Glyph.AddIntersections(intersections, height);

		for (int iIntersection = 0; iIntersection < intersections.size(); iIntersection++)
		{
			const Intersection& intersection{ intersections[iIntersection] };
			const Float3 color{ intersection.rightIsInside ? Float3{0, 1, 0} : Float3{ 1,0,0 } };
			const Float3 position{
				intersection.distance / static_cast<float>(glyphSize.x) * bounds.x + offset.x,
					static_cast<float>((height - m_Glyph.GetMinBounds().y) / glyphSize.y)* bounds.y + offset.y,
					offset.z
			};

			DebugRenderer::AddSphere(position, color, 0.005f);
		}
	}*/
}

MyEngine::Rendering::Image* MyEngine::Io::Ttf::FontRasterizer::MakeImage(const Float3& fillColor) const
{
	using namespace Rendering;
	constexpr float fillAlpha{ 1.0f };
	constexpr float emptyAlpha{ .0f };
	const Float3 emptyColor{ 1, 1, 1 };
	const Double2 glyphSize{ m_Glyph.GetSize() };
	Image* pImage = new Image(m_NrCols, m_NrRows);
	std::vector<Intersection> intersections{};

	for (int iScanLine = 0; iScanLine < m_NrRows; iScanLine++)
	{
		const int imgRow{ m_NrRows - 1 - iScanLine };

		//intersections
		intersections.clear();
		const double height = (static_cast<double>(iScanLine) + 0.5) / static_cast<double>(m_NrRows)
			* glyphSize.y + m_Glyph.GetMinBounds().y;
		m_Glyph.AddIntersections(intersections, height);
		if (intersections.empty())
		{
			pImage->SetRowColor(imgRow, emptyColor, emptyAlpha);
			continue;
		}

		//draw
		int nextIntersectionIdx = 0;
		int nextIntersectionX = static_cast<int>((intersections[0].distance - m_Glyph.GetMinBounds().x) / glyphSize.x * m_NrCols);
		bool inGlyph = false;
		for (int iCol = 0; iCol < m_NrCols; iCol++)
		{
			pImage->SetColor(iCol, imgRow, inGlyph ? fillColor : emptyColor, inGlyph ? 1 : emptyAlpha);
			while (iCol >= nextIntersectionX)
			{
				if (nextIntersectionIdx < intersections.size() - 1)
				{
					inGlyph = intersections[nextIntersectionIdx].rightIsInside;
					nextIntersectionIdx++;
					nextIntersectionX = static_cast<int>((intersections[nextIntersectionIdx].distance - m_Glyph.GetMinBounds().x) / glyphSize.x * m_NrCols);
					if (inGlyph)
						pImage->SetColor(iCol, imgRow, inGlyph ? fillColor : emptyColor, inGlyph ? 1 : emptyAlpha);
				}
				else
				{
					nextIntersectionIdx = std::numeric_limits<int>::max();
					nextIntersectionX = nextIntersectionIdx;
					inGlyph = false;
				}
			}
		}
	}
	return pImage;
}
