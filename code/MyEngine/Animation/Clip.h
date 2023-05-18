#pragma once
#include "DataStructures/GridArray.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				class Model;
				class AnimationLayer;
			}
		}
	}
}

namespace MyEngine
{
	namespace Animation
	{
		class Clip
		{
		public:
			explicit Clip(const Io::Fbx::Wrapping::AnimationLayer& animationLayer);

		private:
			Array<const Io::Fbx::Wrapping::Model*> m_LimbNodes{};
			GridArray<Transform> m_Data{};
		};
	}
}
