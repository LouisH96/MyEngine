#include "pch.h"
#include "FbxTransformCurve.h"

#include "Io/Fbx/Wrapping/AnimationCurveNode.h"
#include "Io/Fbx/Wrapping/Model.h"

using namespace Io::Fbx::Wrapping;

Io::Fbx::FbxTransformCurve::FbxTransformCurve(const Model& limbNode)
{
	FromAnimationCurveNode(*limbNode.GetTranslationCurveNode(), m_TranslationCurves);
	FromAnimationCurveNode(*limbNode.GetRotationCurveNode(), m_RotationCurves);
	FromAnimationCurveNode(*limbNode.GetScaleCurveNode(), m_ScaleCurves);

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

void Io::Fbx::FbxTransformCurve::FromAnimationCurveNode(const AnimationCurveNode& node, FbxValueCurve<float>* pValueCurves)
{
	pValueCurves[0] = FbxValueCurve<float>{ *node.GetAnimationCurves()[0] };
	pValueCurves[1] = FbxValueCurve<float>{ *node.GetAnimationCurves()[1] };
	pValueCurves[2] = FbxValueCurve<float>{ *node.GetAnimationCurves()[2] };
}
