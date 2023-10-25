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
			void Update(InvalidateList<Node>& nodes, const Camera2D& camera);

			bool IsGrabbing() const { return m_Grabbed != Node::INVALID_ID; }
			const Node& GetGrabbedConst(const InvalidateList<Node>& nodes) const;
			Node& GetGrabbed(InvalidateList<Node>& nodes) const;

		private:
			unsigned m_Grabbed{ Uint::Max() };
			Float2 m_GrabOffset;

			static unsigned GetNodeUnderMouse(const InvalidateList<Node>& nodes, const Camera2D& camera);
		};
	}
}
