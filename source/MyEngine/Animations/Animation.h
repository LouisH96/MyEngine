#pragma once
#include "JointsTimeValues.h"
#include "JointsChildren.h"

namespace MyEngine
{
namespace Animations
{
class Animation
{
public:
	struct ModelBuffer
	{
		static constexpr unsigned MAX_BONES = 100;
		Float4X4 BoneTransforms[MAX_BONES];
		Float4X4 World;
	};

	Animation() = default;
	Animation(const Io::Fbx::FbxClass& fbx, const Io::Fbx::FbxAnimation& animation);

	void UpdateModelBuffer(float time);
	const ModelBuffer& GetModelBuffer() const { return m_ModelBuffer; }

private:
	JointsTimeValues m_TimeValues;
	JointsChildren m_ChildrenLookup;
	Array<Quaternion> m_PreAndPostRotations;
	Array<Float4X4> m_BindTransforms;
	Array<unsigned> m_RootJoints;

	ModelBuffer m_ModelBuffer;

	void UpdateTransforms(float time, unsigned iJoint, const Transform& parent);
};
}
}