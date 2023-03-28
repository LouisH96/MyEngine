#pragma once

namespace MyEngine
{
	namespace Generation
	{
		//tip: point of arrow
		//head: thick part of point
		//line: the line
		class ArrowGenerator
		{
		public:
			//---| Types |---
			enum class VertexSection
			{
				LineCap, LineBegin, LineEnd, HeadCap, Head, Tip, Last
			};
			enum class TriangleSection
			{
				LineCap, Line, HeadCap, Head, Last
			};

			ArrowGenerator(int nrSides, bool capLineEnd, float lineLength, float lineRadius, float arrowLength, float arrowRadius);

			int GetNrSides() const { return m_NrSides; }
			
			int GetNrVertices() const;
			int GetNrTriangles() const;

			int GetSectionLength(VertexSection section) const;
			int GetSectionBegin(VertexSection section) const;
			int GetSectionLength(TriangleSection section) const;
			int GetSectionBegin(TriangleSection section) const;

			void Generate(Array<Math::Float3>& vertexPoints, Array<Math::Float3>& vertexNormals, Array<int>& indices) const;

		private:
			const int m_NrSides;
			const bool m_CapLineEnd;
			const float m_LineRadius;
			const float m_LineLength;
			const float m_ArrowRadius;
			const float m_ArrowLength;
		};
	}
}

