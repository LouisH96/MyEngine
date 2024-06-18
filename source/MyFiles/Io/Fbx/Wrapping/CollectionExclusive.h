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
				class Model;

				class CollectionExclusive
				{
				public:
					CollectionExclusive() = default;
					explicit CollectionExclusive(Reading::FbxElement& object);

					int64_t Id{};
					std::string String1;
					std::string String2;

					void AddModel(const Model& model);

				private:
					Array<const Model*> m_Models{};
				};
			}
		}
	}
}
