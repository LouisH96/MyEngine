#pragma once
#include <fstream>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject;

				class FbxReader
				{
				public:
					//---| Construction |---
					FbxReader(const std::wstring& path);
					~FbxReader();

					FbxReader(const FbxReader& other) = delete;
					FbxReader& operator=(const FbxReader& other) = delete;
					FbxReader(FbxReader&& other) = default;
					FbxReader& operator=(FbxReader&& other) = default;

					//---| Functions |---
					const FbxObject& GetRoot() const { return *m_pRoot; }
					FbxObject& GetRoot() { return *m_pRoot; }

				private:
					std::ifstream m_Stream;
					FbxObject* m_pRoot{};

					uint8_t ReadHeader();

					unsigned int ReadUnsignedInt();

					static unsigned int ToUnsignedInt(const char* pBytes);
				};
			}
		}
	}
}
