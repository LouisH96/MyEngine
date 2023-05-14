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
				class AnimationLayer
				{
				public:
					AnimationLayer() = default;
					explicit AnimationLayer(Reading::FbxObject& object);

					int64_t Id;
					std::string Name;
				};
			}
		}
	}
}
