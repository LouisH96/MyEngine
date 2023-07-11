#pragma once
#include <string>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxFile;

				class FbxReader
				{
				public:
					static FbxFile Read(const std::wstring& path);
				};
			}
		}
	}
}
