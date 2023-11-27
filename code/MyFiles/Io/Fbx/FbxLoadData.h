#pragma once
#include <cstdint>

#include "DataStructures/Dictionary.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxClass;

			namespace Wrapping
			{
				class FbxData;
			}

			struct FbxLoadData
			{
				float Scale{};
				FbxClass* pFbxClass{};
				Wrapping::FbxData* pFbxData{};
				Dictionary<uint64_t, unsigned> ModelToJoint{};
			};
		}
	}
}
