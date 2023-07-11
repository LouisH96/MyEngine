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
				class AnimationLayer;

				class AnimationStack
				{
				public:
					AnimationStack() = default;
					explicit AnimationStack(Reading::FbxElement& object);

					int64_t Id{};
					std::string Name{};
					int64_t LocalStart{};
					int64_t LocalStop{};
					int64_t ReferenceStart{};
					int64_t ReferenceStop{};

					void AddAnimationLayer(const AnimationLayer& animationLayer);
					const Array<const AnimationLayer*>& GetAnimationLayers() const { return m_AnimationLayers; }

				private:
					Array<const AnimationLayer*> m_AnimationLayers{};
				};
			}
		}
	}
}
