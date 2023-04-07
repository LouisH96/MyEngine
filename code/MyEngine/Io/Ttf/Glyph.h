#pragma once
#include "Contour.h"
#include "Math/Double2.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class Glyph
			{
			public:
				explicit Glyph(const Array<Array<TtfPoint>>& ttfPoints, const Math::Double2& minBounds, const Math::Double2& maxBounds);

				~Glyph() = default;
				Glyph(const Glyph& other) = default;
				Glyph(Glyph&& other) noexcept = default;
				Glyph& operator=(const Glyph& other) = default;
				Glyph& operator=(Glyph&& other) noexcept = default;

				const Array<Contour>& GetContours() const { return m_Contours; }
				const Math::Double2& GetMinBounds() const { return m_MinBounds; }
				const Math::Double2& GetMaxBounds() const { return m_MaxBounds; }
				Math::Double2 GetSize() const;

				void Translate(const Math::Double2& translation);
				void Scale(double scale);
				void Normalize();

				void DebugDraw(const Math::Float3& color, const Math::Float3& offset, int pointsPerCurve) const;
				void DebugPrint();

			private:
				Array<Contour> m_Contours;
				Math::Double2 m_MinBounds;
				Math::Double2 m_MaxBounds;
			};
		}
	}
}
