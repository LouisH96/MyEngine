#include "pch.h"
#include "FbxGraphMapper.h"

#include "Applied/NodeGraph/Node.h"
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

void FbxGraphMapper::AddModels() const
{
	using namespace Io::Fbx::Wrapping;

	struct NodeModel
	{
		NodeModel() = default;
		explicit NodeModel(int64_t id)
			: FbxId{ id }
			, ParentId{ -1 }
		{}

		explicit NodeModel(const Model& source)
			: FbxId{ source.GetId() }
			, ParentId{ source.HasParent() ? source.GetParentModel()->GetId() : -1 }
		{
		}

		int64_t FbxId;
		int64_t ParentId;
		unsigned NodeId{ Node::INVALID_ID };

		bool operator>(const NodeModel& other) const { return FbxId > other.FbxId; }
		bool operator<(const NodeModel& other) const { return FbxId < other.FbxId; }
		bool operator==(const NodeModel& other) const { return FbxId == other.FbxId; }
		bool operator>=(const NodeModel& other) const { return FbxId >= other.FbxId; }
		bool operator<=(const NodeModel& other) const { return FbxId <= other.FbxId; }

		bool HasParent() const { return ParentId != -1; }
	};

	SortedList<NodeModel> nodeModels{};
	const Array<Model>& sourceModels{ m_FbxData.GetModels() };

	//create nodes
	for (unsigned i = 0; i < sourceModels.GetSize(); i++)
	{
		const Model& sourceModel{ sourceModels[i] };
		NodeModel nodeModel{ sourceModel };
		nodeModel.NodeId = m_Graph.Add({}, 0, {}, sourceModel.GetName());
		nodeModels.TryAdd(nodeModel);
	}

	//create links
	for (unsigned i = 0; i < nodeModels.GetSize(); i++)
	{
		const NodeModel& child{ nodeModels[i] };
		if (!child.HasParent())
			continue;

		const NodeModel& parent{ nodeModels[nodeModels.Find(NodeModel{child.ParentId})] };
		m_Graph.SetParent(child.NodeId, parent.NodeId);
	}
}
