#pragma once

namespace MyEngine
{
class AnimationUtils
{
public:
	static Float3 Blend(
		const Float3& point,
		const Int4& indices, const Float4& weights,
		const Array<Float4X4>& bones);
};
}

