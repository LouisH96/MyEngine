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
			using Generator = RectGenerator<TOPOLOGY>;
			using Vertex = Rendering::V_Pos2Col;

			Node();
			explicit Node(const RectFloat& rect, const Float3& color, const std::string& headerText);

			const RectFloat& GetFullRect() const { return m_FullRect; }
			float GetHeight() const { return m_FullRect.GetHeight(); }
			float GetWidth() const { return m_FullRect.GetWidth(); }

			const RectFloat& GetHeaderRect() const { return m_HeaderRect; }
			void SetHeaderLeftBot(const Float2& leftBot);
			const Float2& GetHeaderLeftBot() const { return m_HeaderRect.GetLeftBot(); }

			void SetColor(const Float3& color) { m_Color = color; }
			const Float3& GetColor() const { return m_Color; }

			void SetParentNode(unsigned parentId);
			bool HasParent() const;
			unsigned GetParentId() const { return m_ParentNode; }

			void SetHeaderTextId(unsigned headerTextId) { m_HeaderTextId = headerTextId;; }
			unsigned GetHeaderTextId() const { return m_HeaderTextId; }

			const std::string& GetHeaderText() const { return m_HeaderText; }

			bool IsValid() const { return m_FullRect.GetLeft() != Float::Max(); }
			void Invalidate() { m_FullRect.SetLeft(Float::Max()); }

		private:
			RectFloat m_FullRect;
			RectFloat m_HeaderRect;
			Float3 m_Color;
			unsigned m_ParentNode{ INVALID_ID };
			std::string m_HeaderText;
			unsigned m_HeaderTextId;

			void UpdatePartialRects();

		public:
			static float HeaderHeight;
			static constexpr float BORDER = .05f;
			static constexpr unsigned NR_RECTS = 3;
			static constexpr unsigned NR_CONNECTION_INDICES = 6;
			static constexpr unsigned NR_CONNECTION_VERTICES = 4;
			static const unsigned NR_INDICES;
			static const unsigned NR_VERTICES;
		};
	}
}
