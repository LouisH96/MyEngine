#include "pch.h"
#include "Clip.h"

#include "JointCurve.h"
#include "Io/Fbx/Wrapping/AnimationCurve.h"
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
	for (int iLimb = 0; iLimb < m_LimbNodes.GetSize(); iLimb++)
	{
		const Model& limb{ *m_LimbNodes[iLimb] };

		const AnimationCurveNode* pTranslationNode{ limb.GetTranslationCurveNode() };
		const AnimationCurveNode* pRotationNode{ limb.GetRotationCurveNode() };
		const AnimationCurveNode* pScaleNode{ limb.GetScaleCurveNode() };
		if (!(pTranslationNode && pRotationNode && pScaleNode)) continue;

		std::cout << "---" << limb.GetName() << "---\n";
		const JointCurve jointCurve{ limb };
	}
}
