#include "pch.h"
#include "TransformCurve.h"

#include "Io/Fbx/Wrapping/AnimationCurve.h"
#include "Io/Fbx/Wrapping/AnimationCurveNode.h"
#include "Io/Fbx/Wrapping/Model.h"

using namespace Io::Fbx::Wrapping;

Animation::TransformCurve::TransformCurve(const Model& limbNode)
{
	FromAnimationCurveNode(*limbNode.GetTranslationCurveNode(), m_TranslationCurves);
	FromAnimationCurveNode(*limbNode.GetRotationCurveNode(), m_RotationCurves);
	FromAnimationCurveNode(*limbNode.GetScaleCurveNode(), m_ScaleCurves);
}

Transform Animation::TransformCurve::AtTime(const int64_t& time) const
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

void Animation::TransformCurve::FromAnimationCurveNode(const AnimationCurveNode& node, ValueCurve<float>* pValueCurves)
{
	pValueCurves[0] = FromAnimationCurve(*node.GetAnimationCurves()[0]);
	pValueCurves[1] = FromAnimationCurve(*node.GetAnimationCurves()[1]);
	pValueCurves[2] = FromAnimationCurve(*node.GetAnimationCurves()[2]);
}

Animation::ValueCurve<float> Animation::TransformCurve::FromAnimationCurve(const AnimationCurve& curve)
{
	return ValueCurve<float>{ SortedArray<int64_t>{curve.KeyTimes},
		SortedArray<float>{curve.KeyValueFloats}};
}
