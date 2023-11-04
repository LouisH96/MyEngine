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

	//move nodes
	MoveNodes();
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
	unsigned insertIdx{ GetLayersFirstNodeIdx(depth) };
	const unsigned lastInsertIdx{ insertIdx + m_Layers[depth].NrNodes };
	while (insertIdx != lastInsertIdx)
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

GraphSorter::SortingNode* GraphSorter::GetFirstChildNode(unsigned nodeIdx, unsigned layer)
{
	const SortingNode* pParent{ GetLayersFirstNode(layer) };
	SortingNode* pChild{ GetLayersFirstNode(layer + 1) };

	while (pParent->NodeId != nodeIdx)
		pChild += pParent++->NrChildren;

	return pChild;
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

void GraphSorter::MoveNodes()
{
	//get highest layers
	float highest{ 0 };
	unsigned highestLayer{ 0 };
	for (unsigned i = 0; i < m_Layers.GetSize(); i++)
	{
		if (m_Layers[i].Size.y <= highest)
			continue;
		highest = m_Layers[i].Size.y;
		highestLayer = i;
	}

	//temp
	OrderSimple(highestLayer);
	for (int i = highestLayer - 1; i >= 0; i--)
		OrderBasedOnNext(i);
}

void GraphSorter::OrderSimple(unsigned layerIdx)
{
	const Layer& layer{ m_Layers[layerIdx] };

	//get start pos
	Float2 pos{ 0, layer.Size.y / 2 };
	for (unsigned i = 0; i < layerIdx; i++)
		pos.x += m_Layers[i].Size.x + HOR_MARGIN;

	//move
	const SortingNode* pNode{ GetLayersFirstNode(layerIdx) };
	const SortingNode* pEnd{ &pNode[layer.NrNodes] };
	while (pNode != pEnd)
	{
		m_pGraph->SetPos(pNode->NodeId, pos, { 0,1 });
		pos.y -= m_pGraph->GetNode(pNode->NodeId).GetHeight() + VER_MARGIN;
		pNode++;
	}
}

void GraphSorter::OrderBasedOnNext(unsigned layerIdx)
{
	const Layer& layer{ m_Layers[layerIdx] };
	const SortingNode* const pFirstNode{ GetLayersFirstNode(layerIdx) };
	const SortingNode* const pEndNode{ &pFirstNode[layer.NrNodes] };

	Float2 pos, pivot;
	pos.x = GetLayerLeft(layerIdx);
	pivot.x = 0;

	//Nodes with children
	const SortingNode* pNode{ pFirstNode };
	for (; pNode != pEndNode; pNode++)
	{
		if (pNode->NrChildren == 0)
			continue;

		const SortingNode* pChild{ GetFirstChildNode(pNode->NodeId, layerIdx) };
		const SortingNode* pLastChild{ pChild + pNode->NrChildren - 1 };

		const Node& firstChild{ m_pGraph->GetNode(pChild->NodeId) };
		const Node& lastChild{ m_pGraph->GetNode(pLastChild->NodeId) };

		const float upper{ firstChild.GetFullRect().GetTop() };
		const float lower{ lastChild.GetFullRect().GetBottom() };

		pos.y = (upper + lower) * .5f;
		pivot.y = .5f;
		m_pGraph->SetPos(pNode->NodeId, pos, pivot);
	}

	//Nodes without children
	pNode = pFirstNode;
	for (; pNode != pEndNode; pNode++)
	{
		if (pNode->NrChildren > 0)
			continue;

		const bool hasNodeAbove{ pNode > pFirstNode };
		const bool hasNodeBelow{ pNode + 1 < pEndNode };

		if (hasNodeAbove)
		{
			const Node& aboveNode{ m_pGraph->GetNode((pNode - 1)->NodeId) };
			const float above{ aboveNode.GetFullRect().GetBottom() };

			if (hasNodeBelow)
			{
				//in between nodes
				const Node& belowNode{ m_pGraph->GetNode((pNode + 1)->NodeId) };
				const float below{ belowNode.GetFullRect().GetTop() };

				pivot.y = .5f;
				pos.y = (above + below) * .5f;
			}
			else
			{
				//last node
				pivot.y = 1;
				pos.y = above - VER_MARGIN;
			}
		}
		else
		{
			if (hasNodeBelow)
			{
				//upper node
				const Node& belowNode{ m_pGraph->GetNode((pNode + 1)->NodeId) };
				const float below{ belowNode.GetFullRect().GetTop() };

				pivot.y = 0;
				pos.y = below + VER_MARGIN;
			}
			else
			{
				//solo node
				//-> this node has no children & this node is only node in layer -> thus there is no next layer
				Logger::PrintError("[GraphSorter::OrderBasedOnNextLayer] this function shouldn't be called if there is no next layer");
			}
		}

		m_pGraph->SetPos(pNode->NodeId, pos, pivot);
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
