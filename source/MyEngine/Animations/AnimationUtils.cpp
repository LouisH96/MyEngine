#include "pch.h"
#include "AnimationUtils.h"

using namespace MyEngine;

Float3 AnimationUtils::Blend(
	const Float3& point,
	const Int4& indices, const Float4& weights,
	const Array<Float4X4>& bones)
{
	Float4X4 bone{};
	for (unsigned i{ 0 }; i < 4; ++i)
	{
		const int& boneIndex{ indices[i] };
		if (boneIndex < 0)
			break;
		bone += bones[boneIndex] * weights[i];
	}
	return bone.AppliedToPoint(point);
}
