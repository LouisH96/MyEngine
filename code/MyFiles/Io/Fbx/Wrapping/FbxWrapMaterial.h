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
				class Model;
				class FbxWrapTexture;

				class FbxWrapMaterial
				{
				public:
					FbxWrapMaterial() = default;
					FbxWrapMaterial(Reading::FbxElement& object);

					int64_t Id;

					void AddModel(const Model& model);
					void AddTexture(const FbxWrapTexture& texture);

				private:
					Array< const Model*> m_Models{};
					Array<const FbxWrapTexture*> m_Textures{};
				};
			}
		}
	}
}
