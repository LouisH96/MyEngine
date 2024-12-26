#pragma once
#include <string>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				class AnimationLayer;
				class AnimationStack;
			}

			class FbxAnimationLayer
			{
			public:
				FbxAnimationLayer() = default;
				explicit FbxAnimationLayer(const Wrapping::AnimationLayer& animationLayer);

				const std::string& GetName() const { return m_Name; }
				int64_t GetId() const { return m_Id; }

			private:
				std::string m_Name{};
				int64_t m_Id{};
			};
		}
	}
}
