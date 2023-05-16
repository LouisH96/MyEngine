#pragma once
#include <cstdint>
#include <string>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject;
			}

			namespace Wrapping
			{
				class AnimationStack;
				class AnimationCurveNode;

				class AnimationLayer
				{
				public:
					AnimationLayer() = default;
					explicit AnimationLayer(Reading::FbxObject& object);

					int64_t Id;
					std::string Name;

					void SetAnimationStack(const AnimationStack& animationStack);
					void AddAnimationCurveNode(const AnimationCurveNode& animationCurveNode);

				private:
					const AnimationStack* m_pAnimationStack{};
					Array<const AnimationCurveNode*> m_AnimationCurveNodes{};
				};
			}
		}
	}
}
