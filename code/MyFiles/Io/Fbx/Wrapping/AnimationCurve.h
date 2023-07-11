#pragma once
#include <cstdint>

#include "DataStructures/Array.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxElement;
			}

			namespace Wrapping
			{
				class AnimationCurveNode;

				class AnimationCurve
				{
				public:
					AnimationCurve() = default;
					explicit AnimationCurve(Reading::FbxElement& object);

					int64_t Id;
					double Default;
					int KeyVer;
					Array<int64_t> KeyTimes;
					Array<float> KeyValueFloats;
					Array<int> KeyAttrFlags;
					Array<float> KeyAttrDataFloat;
					Array<int> KeyAttrRefCount;

					void SetAnimationCurveNode(const AnimationCurveNode& animationCurveNode);

				private:
					const AnimationCurveNode* m_pAnimationCurveNode{};
				};
			}
		}
	}
}
