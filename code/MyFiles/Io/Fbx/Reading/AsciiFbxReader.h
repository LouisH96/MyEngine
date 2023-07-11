#pragma once
#include <istream>

#include "Io/Fbx/Reading/FbxFile.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class AsciiFbxReader
				{
				public:
					static FbxFile Read(std::istream& stream);
				};
			}
		}
	}
}
