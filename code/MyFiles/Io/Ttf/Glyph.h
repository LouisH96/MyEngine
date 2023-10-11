#pragma once
#include "Contour.h"
#include "Intersection.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class Glyph
			{
			public:
				Glyph() = default;
				explicit Glyph(const Array<Array<TtfPoint>>& ttfPoints, const Double2& minBounds, const Double2& maxBounds);

				~Glyph() = default;
				Glyph(const Glyph& other) = default;
				Glyph(Glyph&& other) noexcept = default;
				Glyph& operator=(const Glyph& other) = default;
				Glyph& operator=(Glyph&& other) noexcept = default;

				const Array<Contour>& GetContours() const { return m_Contours; }
				const Double2& GetMinBounds() const { return m_MinBounds; }
				const Double2& GetMaxBounds() const { return m_MaxBounds; }
				Double2 GetSize() const;

				void Translate(const Double2& translation);
				void Scale(double scale);
				void Normalize();

				void AddIntersections(std::vector<Intersection>& intersections, double height) const;

				void DebugDraw(const Float3& color, const Float3& offset, int pointsPerCurve) const;
				void DebugDrawSegments(const Float3& offset, int pointsPerCurve) const;
				void DebugPrint();

				bool IsValid() const { return m_Contours.GetSize() > 0; }

			private:
				Array<Contour> m_Contours;
				Double2 m_MinBounds;
				Double2 m_MaxBounds;
			};
		}
	}
}
