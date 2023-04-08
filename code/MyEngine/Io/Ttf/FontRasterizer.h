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
				explicit FontRasterizer(const Glyph& glyph);

				void DrawBounds(const Math::Float3& color, const Math::Float3& offset) const;
				void DrawGrid(const Math::Float3& offset, const Math::Float2& bounds, int nrCols, int nrRows, const Math::Float3& color) const;

			private:
				const Glyph& m_Glyph;
			};
		}
	}
}
