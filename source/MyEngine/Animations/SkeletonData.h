#pragma once

#include <DataStructures\Array.h>
#include <Math\Matrices.h>
#include <Math\Quaternion.h>

#undef FillMemory

namespace MyEngine
{
namespace Io::Fbx {
class FbxSkeleton;
}

namespace Animations
{
class SkeletonData
{
public:
	struct JointData {
		Float4X4 BindTransform;
	};

	SkeletonData() = default;
	SkeletonData(const Io::Fbx::FbxSkeleton& skeleton);

	const unsigned* GetRootJointsIt(const unsigned*& pFirst) const;
	const unsigned* GetChildrenIt(const unsigned*& pFirst, unsigned iJoint) const;
	const JointData& GetJointData(unsigned iJoint) const { return m_JointData[iJoint]; }

	unsigned FindParent(unsigned iJoint) const;

private:
	//First List of RootJoints
	//Then Lookup-table for each joints towards the beginning of their children (in this array)
	//Then list of children (that gets referenced by the lookup-table)
	Array<unsigned> m_BoneRelations;
	unsigned* m_pLookup{};

	Array<JointData> m_JointData;

	void CreateMemory(const Io::Fbx::FbxSkeleton& skeleton);
	void FillMemory(const Io::Fbx::FbxSkeleton& skeleton);

	unsigned GetNrJoints() const;
};
}
}