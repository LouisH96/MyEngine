#pragma once
#include <cstdint>

#include "DataStructures/Dictionary.h"
#include "Wrapping/FbxOrientation.h"

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
				Wrapping::FbxOrientation Orientation{};
				FbxClass* pFbxClass{};
				Wrapping::FbxData* pFbxData{};
				Dictionary<uint64_t, unsigned> ModelToJoint{};
			};
		}
	}
}
