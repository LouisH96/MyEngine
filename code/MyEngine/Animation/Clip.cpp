#include "pch.h"
#include "Clip.h"

#include "Debug/DebugRenderer.h"
#include "Io/Fbx/Wrapping/AnimationCurve.h"
#include "Io/Fbx/Wrapping/AnimationCurveNode.h"
#include "Io/Fbx/Wrapping/AnimationLayer.h"
#include "Io/Fbx/Wrapping/Model.h"

using namespace Io::Fbx::Wrapping;

Animation::Clip::Clip(const AnimationLayer& animationLayer)
{
	const Array<const AnimationCurveNode*>& curveNodes{ animationLayer.GetCurveNodes() };
	const Model& model{ curveNodes.First()->GetModel().GetRootParentModel() };

	SortedList<int64_t> times{};
	m_LimbNodes = { model.GetChildrenBreadthFirst() };

	//remove LimbNodes without TRS AnimationCurveNodes
	for (int iLimb = 0; iLimb < m_LimbNodes.GetSize();)
	{
		const Model& limb{ *m_LimbNodes[iLimb] };
		const AnimationCurveNode* pTranslationNode{ limb.GetTranslationCurveNode() };
		const AnimationCurveNode* pRotationNode{ limb.GetRotationCurveNode() };
		const AnimationCurveNode* pScaleNode{ limb.GetScaleCurveNode() };
		if (pTranslationNode && pRotationNode && pScaleNode)
		{
			for (int iTime = 0; iTime < pTranslationNode->GetAnimationCurves()[0]->KeyTimes.GetSize(); iTime++)
				times.TryAdd(pTranslationNode->GetAnimationCurves()[0]->KeyTimes[iTime]);
			iLimb++;
			continue;
		}
		m_LimbNodes.RemoveAt(iLimb);
	}

	//
	m_TransformCurves = { m_LimbNodes.GetSize() };
	for (int i = 0; i < m_LimbNodes.GetSize(); i++)
		m_TransformCurves[i] = TransformCurve{ *m_LimbNodes[i] };

	//
	Array<Transform> transforms{m_TransformCurves.GetSize()};
	for (int i = 0; i < m_TransformCurves.GetSize(); i++)
		transforms[i] = m_TransformCurves[i].AtTime(30790772000);

	//draw
	for(int i = 0; i < transforms.GetSize(); i++)
	{
		const Float3 position{ transforms[i].Position*0.01 };
		DebugRenderer::AddSphere(position, { 1,0,0 }, 0.05f);
	}
}
