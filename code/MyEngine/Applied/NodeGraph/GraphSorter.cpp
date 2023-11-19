#include "pch.h"
#include "GraphSorter.h"

#include "NodeGraph.h"

using namespace Applied;

void GraphSorter::Sort(NodeGraph& graph)
{
	GraphSorter{ graph };
}

GraphSorter::GraphSorter(NodeGraph& graph)
	: m_pGraph{ &graph }
{
	const InvalidateList<Node>& nodes{ graph.GetNodes() };

	//make sorted data
	for (unsigned i = nodes.GetFirstIdx(); i < nodes.GetEndIdx(); i++)
		TryAdd(i, m_pGraph->GetDepth(i));

	//Find Heights
	for (unsigned i = 0; i < m_Layers[0].NrNodes; i++)
		FindTotalHeights(m_Nodes[i], 0);

	//move nodes
	MoveHeightBased(&m_Nodes.First(), &m_Nodes[m_Layers.First().NrNodes], {}, 0);
}

void GraphSorter::TryAdd(unsigned nodeIdx, unsigned depth)
{
	//layer
	if (m_Layers.GetSize() <= depth)
		m_Layers.AddMultipleTimes({}, m_Layers.GetSize() - depth + 1);
	else
	{
		//already exist?
		if (HasNode(nodeIdx, depth))
			return;
	}

	const Node& node{ m_pGraph->GetNode(nodeIdx) };

	//add parents
	if (node.HasParent())
	{
		TryAdd(node.GetParentId(), depth - 1);
		GetNode(node.GetParentId(), depth - 1).NrChildren++;
	}

	//add node
	unsigned insertIdx;
	unsigned endInsertIdx;
	if (node.HasParent())
	{
		const SortingNode& parent{ GetNode(node.GetParentId(), depth - 1) };
		insertIdx = GetChildIdx(parent.NodeId, depth - 1);
		endInsertIdx = insertIdx + parent.NrChildren - 1;
	}
	else
	{
		insertIdx = GetLayersFirstNodeIdx(depth);
		endInsertIdx = insertIdx + m_Layers[depth].NrNodes;
	}

	while (insertIdx != endInsertIdx)
	{
		const unsigned currentIdx{ m_Nodes[insertIdx].NodeId };
		if (currentIdx > nodeIdx)
			break;
		insertIdx++;
	}
	m_Nodes.Insert(insertIdx, SortingNode{ nodeIdx, 0 });

	//update layer
	m_Layers[depth].NrNodes++;
	m_Layers[depth].Size.x = Float::Max(m_Layers[depth].Size.x, node.GetWidth());
	m_Layers[depth].Size.y += node.GetHeight() + VER_MARGIN;
}

bool GraphSorter::HasNode(unsigned nodeIdx, unsigned depth) const
{
	if (depth >= m_Layers.GetSize())
		return false;

	const SortingNode* pNode{ GetLayersFirstNode(depth) };
	const SortingNode* const pEnd{ &pNode[m_Layers[depth].NrNodes] };
	while (pNode != pEnd)
	{
		if (pNode->NodeId == nodeIdx)
			return true;
		pNode++;
	}
	return false;
}

const GraphSorter::SortingNode* GraphSorter::GetLayersFirstNode(unsigned layer) const
{
	const SortingNode* pNode{ &m_Nodes[0] };
	for (unsigned i = 0; i < layer; i++)
		pNode += m_Layers[i].NrNodes;
	return pNode;
}

unsigned GraphSorter::GetLayersFirstNodeIdx(unsigned layer) const
{
	unsigned nodeIdx{ 0 };
	while (layer > 0)
		nodeIdx += m_Layers[--layer].NrNodes;
	return nodeIdx;
}

GraphSorter::SortingNode* GraphSorter::GetLayersFirstNode(unsigned layer)
{
	SortingNode* pNode{ &m_Nodes[0] };
	for (unsigned i = 0; i < layer; i++)
		pNode += m_Layers[i].NrNodes;
	return pNode;
}

GraphSorter::SortingNode& GraphSorter::GetNode(unsigned nodeIdx, unsigned depth)
{
	SortingNode* pNode{ GetLayersFirstNode(depth) };
	while (pNode->NodeId != nodeIdx)
		pNode++;
	return *pNode;
}

GraphSorter::SortingNode* GraphSorter::GetChild(unsigned nodeIdx, unsigned layer)
{
	const SortingNode* pParent{ GetLayersFirstNode(layer) };
	SortingNode* pChild{ GetLayersFirstNode(layer + 1) };

	while (pParent->NodeId != nodeIdx)
		pChild += pParent++->NrChildren;

	return pChild;
}

unsigned GraphSorter::GetChildIdx(unsigned nodeIdx, unsigned layer)
{
	return GetChild(nodeIdx, layer) - m_Nodes.GetData();
}

unsigned GraphSorter::GetDepth(const Node& node) const
{
	unsigned depth{ 0 };

	const Node* pNode{ &node };
	while (pNode->HasParent())
	{
		depth++;
		pNode = &m_pGraph->GetNodes().Get(pNode->GetParentId());
	}

	return depth;
}

float GraphSorter::GetLayerLeft(unsigned layer) const
{
	float left{ 0 };
	for (unsigned i = 0; i < layer; i++)
		left += m_Layers[i].Size.x + HOR_MARGIN;
	return left;
}

float GraphSorter::FindTotalHeights(SortingNode& node, unsigned layer)
{
	SortingNode* pChild{ GetChild(node.NodeId, layer) };
	const SortingNode* pEndChild{ pChild + node.NrChildren };

	for (; pChild != pEndChild; pChild++)
		node.TotalHeight += FindTotalHeights(*pChild, layer + 1) + VER_MARGIN;

	const Node& sourceNode{ m_pGraph->GetNode(node.NodeId) };
	node.TotalHeight = Float::Max(sourceNode.GetHeight(), node.TotalHeight);
	return node.TotalHeight;
}

void GraphSorter::MoveHeightBased(const SortingNode* pNode, const SortingNode* pNodeEnd, Float2 leftCenter, unsigned layer)
{
	const SortingNode* const pFirst{ pNode };

	const Float2 pivot{ 0,.5f };
	Float2 pos{ leftCenter.x, 0 };

	//get layer height
	for (; pNode != pNodeEnd; pNode++)
		pos.y += pNode->TotalHeight + VER_MARGIN;
	pos.y -= VER_MARGIN;

	//move
	pos.y /= 2;
	pos.y += leftCenter.y;
	for (pNode = pFirst; pNode != pNodeEnd; pNode++)
	{
		pos.y -= pNode->TotalHeight / 2.f;
		m_pGraph->SetPos(pNode->NodeId, pos, pivot);

		//move children
		if (pNode->NrChildren > 0)
		{
			const SortingNode* pChildFirst{ GetChild(pNode->NodeId, layer) };
			const SortingNode* pChildEnd{ pChildFirst + pNode->NrChildren };
			const Float2 childrenPos{ leftCenter.x + m_Layers[layer].Size.x + HOR_MARGIN, pos.y };
			MoveHeightBased(pChildFirst, pChildEnd, childrenPos, layer + 1);
		}

		pos.y -= pNode->TotalHeight / 2.f + VER_MARGIN;
	}
}

void GraphSorter::Print() const
{
	const SortingNode* pNode{ &m_Nodes[0] };
	for (unsigned iLayer = 0; iLayer < m_Layers.GetSize(); iLayer++)
	{
		const Layer& layer{ m_Layers[iLayer] };
		std::cout << "Layer" << iLayer << ": " << layer.NrNodes << " nodes\n";

		for (unsigned iNode = 0; iNode < layer.NrNodes; iNode++)
		{
			std::cout << " Node" << pNode->NodeId << ": " << pNode->NrChildren << " children\n";
			pNode++;
		}
	}
}
