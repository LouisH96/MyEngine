#include "Pose.h"

#include "Io/Fbx/Reading/FbxElement.h"
#include "Io/Fbx/Reading/Properties/FbxPropArray.h"
#include "Io/Fbx/Reading/Properties/FbxPropPrimitive.h"

MyEngine::Io::Fbx::Wrapping::Pose::Pose(Reading::FbxElement& poseObject)
	: Name{ poseObject.GetProperty(1).AsString() }
	, Type{ poseObject.GetProperty(2).AsString() }
	, Nodes{ poseObject.GetChild("NbPoseNodes")->GetProperty(0).AsPrimitive<int>().GetValue() }
{
	List<Reading::FbxElement*> nodes{ poseObject.GetChildren("PoseNode") };
	for (int iNode = 0; iNode < Nodes.GetSize(); iNode++)
	{
		Reading::FbxElement& readNode{ *nodes[iNode] };
		Node& dataNode{ Nodes[iNode] };
		dataNode.Id = readNode.GetChild("Node")->GetProperty(0).AsPrimitive<int64_t>().GetValue();
		const Array<double>& array{readNode.GetChild("Matrix")->GetProperty(0).AsArray<double>().GetValues()};
		dataNode.Matrix = Double4X4::FromRowMajor(array.GetData());
	}
}
