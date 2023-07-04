#pragma once
#include "Math/Vectors.h"
#include "MyFilesGlobals.h"

namespace MyEngine
{
	namespace Rendering
	{
		class Image;
	}

	namespace Io
	{
		namespace Ttf
		{
			class Glyph;

			class MY_FILES_API FontRasterizer
			{
			public:
				explicit FontRasterizer(const Glyph& glyph, const Int2& sizeInPixels);
				explicit FontRasterizer(const Glyph& glyph, int nrCols, int nrRows);
				~FontRasterizer() = default;

				void DrawBounds(const Float3& color, const Float3& offset) const;
				void DrawGrid(const Float3& offset, const Float3& color) const;
				void DrawIntersections(const Float3& offset, const Float2& bounds) const;
				Rendering::Image* MakeImage(const Float3& fillColor) const;

				static void DeleteImage(Rendering::Image* pImage);

			private:
				const Glyph& m_Glyph;
				const int m_NrCols;
				const int m_NrRows;
			};
		}
	}
}
