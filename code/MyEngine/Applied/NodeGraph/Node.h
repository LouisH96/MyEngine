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
			static constexpr unsigned INVALID_ID{ Uint::MAX };
			using Vertex = Rendering::V_Pos2Col;

			Node();
			explicit Node(const RectFloat& rect, const Float3& color);

			const RectFloat& GetFullRect() const { return m_FullRect; }

			const RectFloat& GetHeaderRect() const { return m_HeaderRect; }
			void SetHeaderLeftBot(const Float2& leftBot);

			void SetColor(const Float3& color) { m_Color = color; }
			const Float3& GetColor() const { return m_Color; }

			void SetParentNode(unsigned parentId);
			bool HasParent() const;

			void WriteVertices(Vertex*& pVertices) const;
			static void WriteIndices(int*& pIndices, unsigned offset);

			void WriteConnectionVertices(Vertex*& pVertices, const Node* pNodes) const;
			void WriteConnectionIndices(int*& pIndices, unsigned offset) const;

			bool IsValid() const { return m_FullRect.GetLeft() != Float::Max(); }
			void Invalidate() { m_FullRect.SetLeft(Float::Max()); }

		private:
			using Generator = RectGenerator<TOPOLOGY>;
			RectFloat m_FullRect;
			RectFloat m_HeaderRect;
			Float3 m_Color;
			unsigned m_ParentNode{ INVALID_ID };

			void UpdatePartialRects();
			static void WriteVertices(Vertex*& pVertices, const RectFloat& rect, const Float3& color);

		public:
			static constexpr float HEADER_HEIGHT = .5f;
			static constexpr float BORDER = .05f;
			static constexpr unsigned NR_RECTS = 3;
			static constexpr unsigned NR_CONNECTION_INDICES = 6;
			static constexpr unsigned NR_CONNECTION_VERTICES = 4;
			static const unsigned NR_INDICES;
			static const unsigned NR_VERTICES;
		};
	}
}
