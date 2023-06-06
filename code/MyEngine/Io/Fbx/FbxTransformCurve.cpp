#include "pch.h"
#include "FbxTransformCurve.h"

#include "Io/Fbx/Wrapping/AnimationCurveNode.h"
#include "Io/Fbx/Wrapping/Model.h"
#include "Io/Fbx/FbxANimationLayer.h"

using namespace Io::Fbx::Wrapping;

Io::Fbx::FbxTransformCurve::FbxTransformCurve(const Wrapping::Model& limbNode, const FbxAnimationLayer& layer)
	: m_pLayer{ &layer }
	, m_TranslationCurves{}, m_RotationCurves{}, m_ScaleCurves{}
{
	const AnimationCurveNode* pTranslationNode{ limbNode.GetTranslationCurveNode(m_pLayer->GetId()) };
	if (pTranslationNode) FromAnimationCurveNode(*pTranslationNode, m_TranslationCurves);
	else FromDefaultValue(limbNode.GetLclTranslation(), m_TranslationCurves);

	const AnimationCurveNode* pRotationNode{ limbNode.GetRotationCurveNode(m_pLayer->GetId()) };
	if (pRotationNode) FromAnimationCurveNode(*pRotationNode, m_RotationCurves);
	else FromDefaultValue(limbNode.GetLclRotation(), m_RotationCurves);

	const AnimationCurveNode* pScaleNode{ limbNode.GetScaleCurveNode(m_pLayer->GetId()) };
	if (pScaleNode) FromAnimationCurveNode(*pScaleNode, m_ScaleCurves);
	else FromDefaultValue(limbNode.GetLclScaling(), m_ScaleCurves);

	m_TranslationCurves[0].ScaleValues(-1);
	m_RotationCurves[1].ScaleValues(-1);
	m_RotationCurves[2].ScaleValues(-1);
	m_ScaleCurves[0].ScaleValues(-1);
}

Transform Io::Fbx::FbxTransformCurve::AtTime(const int64_t& time) const
{
	const Float3 translation{
		m_TranslationCurves[0].ValueAtTime(time),
		m_TranslationCurves[1].ValueAtTime(time),
		m_TranslationCurves[2].ValueAtTime(time)
	};
	const Float3 rotation{
		m_RotationCurves[0].ValueAtTime(time),
		m_RotationCurves[1].ValueAtTime(time),
		m_RotationCurves[2].ValueAtTime(time)
	};
	return Transform{ translation, Quaternion::FromEulerDegrees(rotation) };
}

bool Io::Fbx::FbxTransformCurve::IsInLayer(const FbxAnimationLayer& layer) const
{
	return &layer == m_pLayer;
}

void Io::Fbx::FbxTransformCurve::FromAnimationCurveNode(const AnimationCurveNode& node, FbxValueCurve<float>* pValueCurves)
{
	if (node.GetAnimationCurves().GetSize() == 0)
	{
		FromDefaultValue(node.GetDefaultVector(), pValueCurves);
		return;
	}
	pValueCurves[0] = FbxValueCurve<float>{ *node.GetAnimationCurves()[0] };
	pValueCurves[1] = FbxValueCurve<float>{ *node.GetAnimationCurves()[1] };
	pValueCurves[2] = FbxValueCurve<float>{ *node.GetAnimationCurves()[2] };
}

void Io::Fbx::FbxTransformCurve::FromDefaultValue(const Double3& value, FbxValueCurve<float>* pValueCurves)
{
	pValueCurves[0] = FbxValueCurve<float>{ static_cast<float>(value.x) };
	pValueCurves[1] = FbxValueCurve<float>{ static_cast<float>(value.y) };
	pValueCurves[2] = FbxValueCurve<float>{ static_cast<float>(value.z) };
}

void Io::Fbx::FbxTransformCurve::FromDefaultValue(const Float3& value, FbxValueCurve<float>* pValueCurves)
{
	pValueCurves[0] = FbxValueCurve<float>{ value.x };
	pValueCurves[1] = FbxValueCurve<float>{ value.y };
	pValueCurves[2] = FbxValueCurve<float>{ value.z };
}
