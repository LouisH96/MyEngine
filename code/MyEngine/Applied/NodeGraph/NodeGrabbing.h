#pragma once
#include "Node.h"
#include "DataStructures/InvalidateList.h"

namespace MyEngine
{
	namespace Applied
	{
		class NodeGrabbing
		{
		public:
			struct NodeMoved
			{
				unsigned NodeId{ Node::INVALID_ID };
				Float2 Displacement;

				bool HasMoved() const { return NodeId != Node::INVALID_ID; }
			};

			NodeMoved Update(const InvalidateList<Node>& nodes, const Camera2D& camera);

			bool IsGrabbing() const { return m_Grabbed != Node::INVALID_ID; }
			const Node& GetGrabbed(const InvalidateList<Node>& nodes) const;

		private:
			unsigned m_Grabbed{ Uint::Max() };
			Float2 m_GrabOffset;

			static unsigned GetNodeUnderMouse(const InvalidateList<Node>& nodes, const Camera2D& camera);
		};
	}
}
