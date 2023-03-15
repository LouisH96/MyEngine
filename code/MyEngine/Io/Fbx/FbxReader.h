#pragma once
#include <fstream>
#include <vector>

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxReader
			{
			public:
				FbxReader(const std::wstring& path);

			private:
				std::ifstream m_Stream;
				bool m_AtEnd{ false };

				void ReadHeader();
				void ReadNode();

				unsigned int ReadUnsignedInt();

				static unsigned int ToUnsignedInt(const char* pBytes);
			};
		}
	}
}
