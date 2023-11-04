#pragma once
#include "Node.h"

namespace MyEngine
{
	namespace Applied
	{
		class Node;
		class NodeGraph;

		class GraphSorter
		{
		public:
			static void Sort(NodeGraph& graph);

			explicit GraphSorter(NodeGraph& graph);

		private:
			struct Layer
			{
				unsigned NrNodes{ 0 };
				Float2 Size{};
			};
			struct SortingNode
			{
				unsigned NodeId{ Node::INVALID_ID };
				unsigned NrChildren{ 0 };
			};
			static constexpr float HOR_MARGIN{ .8f };
			static constexpr float VER_MARGIN{ .3f };
			NodeGraph* m_pGraph;
			List<Layer> m_Layers;
			List<SortingNode> m_Nodes;

			void TryAdd(unsigned nodeIdx, unsigned depth);
			bool HasNode(unsigned nodeIdx, unsigned depth) const;
			const SortingNode* GetLayersFirstNode(unsigned layer) const;
			unsigned GetLayersFirstNodeIdx(unsigned layer) const;
			SortingNode* GetLayersFirstNode(unsigned layer);
			SortingNode& GetNode(unsigned nodeIdx, unsigned depth);
			SortingNode* GetFirstChildNode(unsigned nodeIdx, unsigned layer);
			unsigned GetDepth(const Node& node) const;
			float GetLayerLeft(unsigned layer) const;

			void MoveNodes();
			void OrderSimple(unsigned layerIdx);
			void OrderBasedOnNext(unsigned layerIdx);

			void Print() const;
		};
	}
}
