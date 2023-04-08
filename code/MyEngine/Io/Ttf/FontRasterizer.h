#pragma once
#include "Math/Float2.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class Glyph;

			class FontRasterizer
			{
			public:
				explicit FontRasterizer(Glyph& glyph, int nrCols, int nrRows);

				void DrawBounds(const Math::Float3& color, const Math::Float3& offset) const;
				void DrawGrid(const Math::Float3& offset, const Math::Float3& color) const;
				void DrawIntersections(const Math::Float3& offset) const;

			private:
				Glyph& m_Glyph;
				const int m_NrCols;
				const int m_NrRows;
			};
		}
	}
}
