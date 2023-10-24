#pragma once
#include "Generation/RectGenerator.h"
#include "Geometry/Shapes/Rects.h"

namespace MyEngine
{
	namespace Applied
	{
		class Node
		{
		public:
			static constexpr ModelTopology TOPOLOGY{ ModelTopology::TriangleListIdx };
			using Vertex = Rendering::V_Pos2Col;

			static constexpr unsigned NR_INDICES = 6;
			static constexpr unsigned NR_VERTICES = 4;

			Node();
			explicit Node(const RectFloat& rect, const Float3& color);

			const RectFloat& GetRect() const { return m_Rect; }

			void SetColor(const Float3& color) { m_Color = color; }
			const Float3& GetColor() const { return m_Color; }

			void WriteVertices(Vertex*& pVertices) const;
			static void WriteIndices(int*& pIndices, unsigned offset);

			bool IsValid() const { return m_Rect.GetLeft() != Float::Max(); }
			void Invalidate() { m_Rect.SetLeft(Float::Max()); }

		private:
			using Generator = RectGenerator<TOPOLOGY>;
			RectFloat m_Rect;
			Float3 m_Color;
		};
	}
}
