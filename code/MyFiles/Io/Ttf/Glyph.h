#pragma once
#include "Contour.h"
#include "Intersection.h"
#include "MyFilesGlobals.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class Glyph
			{
			public:
				MY_FILES_API explicit Glyph(const Array<Array<TtfPoint>>& ttfPoints, const Double2& minBounds, const Double2& maxBounds);

				MY_FILES_API ~Glyph() = default;
				MY_FILES_API Glyph(const Glyph& other) = default;
				MY_FILES_API Glyph(Glyph&& other) noexcept = default;
				MY_FILES_API Glyph& operator=(const Glyph& other) = default;
				MY_FILES_API Glyph& operator=(Glyph&& other) noexcept = default;

				MY_FILES_API const Array<Contour>& GetContours() const { return m_Contours; }
				MY_FILES_API const Double2& GetMinBounds() const { return m_MinBounds; }
				MY_FILES_API const Double2& GetMaxBounds() const { return m_MaxBounds; }
				MY_FILES_API Double2 GetSize() const;

				MY_FILES_API void Translate(const Double2& translation);
				MY_FILES_API void Scale(double scale);
				MY_FILES_API void Normalize();

				MY_FILES_API void AddIntersections(std::vector<Intersection>& intersections, double height) const;

				MY_FILES_API void DebugDraw(const Float3& color, const Float3& offset, int pointsPerCurve) const;
				MY_FILES_API void DebugDrawSegments(const Float3& offset, int pointsPerCurve) const;
				MY_FILES_API void DebugPrint();

			private:
				Array<Contour> m_Contours;
				Double2 m_MinBounds;
				Double2 m_MaxBounds;
			};
		}
	}
}
