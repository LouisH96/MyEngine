#pragma once
#include <cstdint>
#include <string>

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
				class AnimationStack;
				class AnimationCurveNode;

				class AnimationLayer
				{
				public:
					AnimationLayer() = default;
					explicit AnimationLayer(Reading::FbxElement& object);

					int64_t Id;
					std::string Name;

					void SetAnimationStack(const AnimationStack& animationStack);
					void AddAnimationCurveNode(const AnimationCurveNode& animationCurveNode);

					const Array<const AnimationCurveNode*>& GetCurveNodes() const { return m_AnimationCurveNodes; }

				private:
					const AnimationStack* m_pAnimationStack{};
					Array<const AnimationCurveNode*> m_AnimationCurveNodes{};
				};
			}
		}
	}
}
