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
				using JointLookup = Dictionary<uint64_t, unsigned>;

				Wrapping::FbxOrientation Orientation{};
				FbxClass* pFbxClass{};
				Wrapping::FbxData* pFbxData{};
				JointLookup ModelToJoint{};
			};
		}
	}
}
