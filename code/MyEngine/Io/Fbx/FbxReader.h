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

			private:
				std::ifstream m_Stream;
				FbxObject* m_pRoot{};

				void ReadHeader();

				unsigned int ReadUnsignedInt();

				static unsigned int ToUnsignedInt(const char* pBytes);
			};
		}
	}
}
