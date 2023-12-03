#include "Pose.h"

#include "Io/Fbx/Reading/FbxElement.h"
#include "Io/Fbx/Reading/Properties/FbxPropArray.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

using namespace MyEngine;
using namespace Io::Fbx::Wrapping;

Pose::Pose(Reading::FbxElement& poseObject)
	: Name{ poseObject.GetProperty(1).AsString() }
	, Type{ poseObject.GetProperty(2).AsString() }
	, Nodes{ poseObject.GetChild("NbPoseNodes")->GetProperty(0).AsPrimitive<int>().GetValue() }
{
	List<Reading::FbxElement*> nodes{ poseObject.GetChildren("PoseNode") };
	for (unsigned iNode = 0; iNode < Nodes.GetSize(); iNode++)
	{
		Reading::FbxElement& readNode{ *nodes[iNode] };
		Node& dataNode{ Nodes[iNode] };
		dataNode.Id = readNode.GetChild("Node")->GetProperty(0).AsPrimitive<int64_t>().GetValue();
		const Array<double>& array{readNode.GetChild("Matrix")->GetProperty(0).AsArray<double>().GetValues()};
		dataNode.Matrix = Double4X4::FromRowMajor(array.GetData());
	}
}

const Pose::Node* Pose::FindNode(const int64_t& id) const
{
	for (unsigned i = 0; i < Nodes.GetSize(); i++)
		if (Nodes[i].Id == id)
			return &Nodes[i];
	return nullptr;
}

const Double4X4* Pose::FindMatrix(const int64_t& id) const
{
	return &FindNode(id)->Matrix;
}
