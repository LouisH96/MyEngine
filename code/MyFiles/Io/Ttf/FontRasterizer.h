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

			class FontRasterizer
			{
			public:
				MY_FILES_API explicit FontRasterizer(const Glyph& glyph, const Int2& sizeInPixels);
				MY_FILES_API explicit FontRasterizer(const Glyph& glyph, int nrCols, int nrRows);

				MY_FILES_API void DrawBounds(const Float3& color, const Float3& offset) const;
				MY_FILES_API void DrawGrid(const Float3& offset, const Float3& color) const;
				MY_FILES_API void DrawIntersections(const Float3& offset, const Float2& bounds) const;
				MY_FILES_API Rendering::Image* MakeImage(const Float3& fillColor) const;

			private:
				const Glyph& m_Glyph;
				const int m_NrCols;
				const int m_NrRows;
			};
		}
	}
}
