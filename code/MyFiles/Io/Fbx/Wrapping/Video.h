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
				class FbxWrapTexture;

				class Video
				{
				public:
					Video() = default;
					explicit Video(Reading::FbxElement& object);

					int64_t Id{};

					void AddTexture(const FbxWrapTexture& texture);

				private:
					Array<const FbxWrapTexture*> m_Textures{};
				};
			}
		}
	}
}
