#include "pch.h"
#include "FbxGraphMapper.h"

#include "Applied/NodeGraph/NodeGraph.h"
#include "DataStructures/SortedList.h"
#include "Io/Fbx/Wrapping/FbxData.h"

using namespace Applied;

FbxGraphMapper::FbxGraphMapper(const Io::Fbx::Wrapping::FbxData& fbx, NodeGraph& graph)
	: m_FbxData{ fbx }
	, m_Graph{ graph }
{
	AddModels();
	m_Graph.AutoStructure();
}

FbxGraphMapper::ModelNode::ModelNode(int64_t id)
	: FbxId{ id }
	, ParentId{ -1 }
{}

FbxGraphMapper::ModelNode::ModelNode(const Io::Fbx::Wrapping::Model& source)
	: FbxId{ source.GetId() }
	, ParentId{ source.HasParent() ? source.GetParentModel()->GetId() : -1 }
{
}

void FbxGraphMapper::AddModels()
{
	using namespace Io::Fbx::Wrapping;
	const Array<Model>& sourceModels{ m_FbxData.GetModels() };

	//create nodes
	for (unsigned i = 0; i < sourceModels.GetSize(); i++)
	{
		const Model& sourceModel{ sourceModels[i] };
		ModelNode nodeModel{ sourceModel };
		nodeModel.NodeId = m_Graph.Add({}, 0, {}, sourceModel.GetName());
		m_ModelNodes.TryAdd(nodeModel);
	}

	//create links
	for (unsigned i = 0; i < m_ModelNodes.GetSize(); i++)
	{
		const ModelNode& child{ m_ModelNodes[i] };
		if (!child.HasParent())
			continue;

		const ModelNode& parent{ m_ModelNodes[m_ModelNodes.Find(ModelNode{child.ParentId})] };
		m_Graph.SetParent(child.NodeId, parent.NodeId);
	}
}
