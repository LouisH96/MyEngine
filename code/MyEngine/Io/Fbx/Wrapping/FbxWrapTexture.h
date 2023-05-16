#pragma once

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
				class Video;

				class FbxWrapTexture
				{
				public:
					FbxWrapTexture() = default;
					explicit FbxWrapTexture(Reading::FbxObject& object);

					int64_t Id;

					void AddLinkedVideo(const Video& video);

				private:
					Array<const Video*> m_LinkedVideos{};
				};
			}
		}
	}
}
