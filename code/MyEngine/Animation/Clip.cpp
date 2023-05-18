#include "pch.h"
#include "Clip.h"

#include "Io/Fbx/Wrapping/AnimationCurveNode.h"
#include "Io/Fbx/Wrapping/AnimationLayer.h"
#include "Io/Fbx/Wrapping/Model.h"

using namespace Io::Fbx::Wrapping;

Animation::Clip::Clip(const AnimationLayer& animationLayer)
{
	const Array<const AnimationCurveNode*>& curveNodes{ animationLayer.GetCurveNodes() };
	const Model& model{ curveNodes.First()->GetModel().GetRootParentModel() };

	m_LimbNodes = { model.GetChildrenBreadthFirst() };

	//remove LimbNodes without TRS AnimationCurveNodes
	for (int iLimb = 0; iLimb < m_LimbNodes.GetSize();)
	{
		const Model& limb{ *m_LimbNodes[iLimb] };
		const Array<const AnimationCurveNode*>& curveNodes{ limb.GetAnimationCurveNodes() };

		if (curveNodes.None([](const AnimationCurveNode* pNode) {
			return pNode->GetNodeType() == AnimationCurveNode::NodeType::Translation; }))
		{
			m_LimbNodes.RemoveAt(iLimb);
			continue;
		}
			if (curveNodes.None([](const AnimationCurveNode* pNode) {
				return pNode->GetNodeType() == AnimationCurveNode::NodeType::Rotation; }))
			{
				m_LimbNodes.RemoveAt(iLimb);
				continue;
			}
				if (curveNodes.None([](const AnimationCurveNode* pNode) {
					return pNode->GetNodeType() == AnimationCurveNode::NodeType::Scale; }))
				{
					m_LimbNodes.RemoveAt(iLimb);
					continue;
				}
					iLimb++;
	}

	for (int iLimb = 0; iLimb < m_LimbNodes.GetSize(); iLimb++)
	{
		const Model& limb{ *m_LimbNodes[iLimb] };
		const Array<const AnimationCurveNode*>& curveNodes{ limb.GetAnimationCurveNodes() };
		std::cout << limb.GetName() << ": " << curveNodes.GetSize() << std::endl;
	}
}
