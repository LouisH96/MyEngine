#include "pch.h"
#include "JointCurve.h"

#include "DataStructures/SortedList.h"
#include "Io/Fbx/Wrapping/AnimationCurve.h"
#include "Io/Fbx/Wrapping/AnimationCurveNode.h"
#include "Io/Fbx/Wrapping/Model.h"

using namespace Io::Fbx::Wrapping;

Animation::JointCurve::JointCurve(const Model& model)
{
	const AnimationCurveNode* pTranslationNode{ model.GetTranslationCurveNode() };
	const AnimationCurveNode* pRotationNode{ model.GetRotationCurveNode() };
	const AnimationCurveNode* pScaleNode{ model.GetScaleCurveNode() };

	SortedList<int64_t> times{pTranslationNode->GetAnimationCurves()[0]->KeyTimes.GetSize()};
	AddTimes(times, *pTranslationNode);
	AddTimes(times, *pRotationNode);
	AddTimes(times, *pScaleNode);

	for (int i = 0; i < times.GetSize(); i++)
		std::cout << times[i] << std::endl;


}

void Animation::JointCurve::AddTimes(SortedList<int64_t>& list, const Io::Fbx::Wrapping::AnimationCurveNode& node)
{
	AddTimes(list, *node.GetAnimationCurves()[0]);
	AddTimes(list, *node.GetAnimationCurves()[1]);
	AddTimes(list, *node.GetAnimationCurves()[2]);
}

void Animation::JointCurve::AddTimes(SortedList<int64_t>& list, const Io::Fbx::Wrapping::AnimationCurve& curve)
{
	for (int i = 0; i < curve.KeyTimes.GetSize(); i++)
		list.TryAdd(curve.KeyTimes[i]);
}
