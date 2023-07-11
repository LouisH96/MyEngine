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
				class FbxElement;
				class FbxFile;

				class BinaryFbxReader
				{
				public:
					static FbxFile Read(const std::wstring& path);

				private:
					struct ElementInfo
					{
						std::streampos End;
						uint64_t NrProperties;
						uint64_t PropertiesLength;
					};
					static ElementInfo ReadElementInfo(std::istream& stream, const FbxFile& file);

					static void ReadProperties(std::istream& stream, FbxElement& parent, const ElementInfo& info, const FbxFile& file);
					static void ReadChildren(std::istream& stream, FbxElement& parent, const ElementInfo& info, const FbxFile& file);
				};
			}
		}
	}
}
