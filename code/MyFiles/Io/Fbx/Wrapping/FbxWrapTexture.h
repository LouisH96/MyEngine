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
				class FbxWrapMaterial;
				class Video;

				class FbxWrapTexture
				{
				public:
					FbxWrapTexture() = default;
					explicit FbxWrapTexture(Reading::FbxElement& object);

					int64_t Id;

					void AddMaterial(const FbxWrapMaterial& material);
					void AddLinkedVideo(const Video& video);

				private:
					Array<const FbxWrapMaterial*> m_Materials{};
					Array<const Video*> m_LinkedVideos{};
				};
			}
		}
	}
}
