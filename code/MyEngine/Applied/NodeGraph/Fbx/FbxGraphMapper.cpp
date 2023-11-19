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
	AddGeometries();
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

void FbxGraphMapper::AddGeometries() const
{
	using namespace Io::Fbx::Wrapping;
	const Array<Geometry>& source{ m_FbxData.GetGeometries() };

	for (unsigned i = 0; i < source.GetSize(); i++)
	{
		const Geometry& geometry{ source[i] };
		const ModelNode& parent{ GetModelNode(geometry.GetRootModelId()) };

		std::string name{ geometry.GetName() };
		if (name.empty())
			name = "Geometry";

		const int nodeId{ m_Graph.Add({}, 0, {}, name, { .2f,.6f, .2f }) };
		m_Graph.SetParent(nodeId, parent.NodeId);
	}
}

const FbxGraphMapper::ModelNode& FbxGraphMapper::GetModelNode(int64_t modelId) const
{
	return m_ModelNodes[m_ModelNodes.Find(ModelNode{ modelId })];
}
