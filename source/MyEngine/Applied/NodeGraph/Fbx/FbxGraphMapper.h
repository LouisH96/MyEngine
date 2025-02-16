#pragma once
#include "Applied/NodeGraph/Node.h"
#include "DataStructures/SortedList.h"

namespace MyEngine
{
namespace Io
{
namespace Fbx
{
namespace Wrapping
{
class Model;
class FbxData;
}
}
}
}

namespace MyEngine
{
namespace Applied
{
class NodeGraph;

//class FbxGraphMapper
//{
//public:
//	FbxGraphMapper(const Io::Fbx::Wrapping::FbxData& fbx, NodeGraph& graph);
//
//private:
//	struct ModelNode
//	{
//		ModelNode() = default;
//		explicit ModelNode(int64_t id);
//		explicit ModelNode(const Io::Fbx::Wrapping::Model& source);
//
//		int64_t FbxId;
//		int64_t ParentId;
//		unsigned NodeId{ Node::INVALID_ID };
//
//		bool operator>(const ModelNode& other) const { return FbxId > other.FbxId; }
//		bool operator<(const ModelNode& other) const { return FbxId < other.FbxId; }
//		bool operator==(const ModelNode& other) const { return FbxId == other.FbxId; }
//		bool operator>=(const ModelNode& other) const { return FbxId >= other.FbxId; }
//		bool operator<=(const ModelNode& other) const { return FbxId <= other.FbxId; }
//
//		bool HasParent() const { return ParentId != -1; }
//	};
//
//	const Io::Fbx::Wrapping::FbxData& m_FbxData;
//	NodeGraph& m_Graph;
//	SortedList<ModelNode> m_ModelNodes{};
//
//	void AddModels();
//	void AddGeometries() const;
//	void AddAnimation() const;
//
//	const ModelNode& GetModelNode(int64_t modelId) const;
//};
}
}
