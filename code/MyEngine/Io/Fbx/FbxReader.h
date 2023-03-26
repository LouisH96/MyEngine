#pragma once
#include <fstream>
#include <vector>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxObject;

			class FbxReader
			{
			public:
				FbxReader(const std::wstring& path);
				~FbxReader();

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
