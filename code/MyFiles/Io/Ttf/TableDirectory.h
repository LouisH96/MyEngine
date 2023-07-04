#pragma once
#include <cstdint>
#include <string>

namespace MyEngine
{
	namespace Io
	{
		namespace Bin
		{
			class BigBinReader;
		}

		namespace Ttf
		{
			class TableDirectory
			{
			public:
				void Read(const Bin::BigBinReader& reader);
				void Print() const;
				static void PrintHeader();

				std::string GetTag() const;
				uint32_t GetOffset() const { return m_Offset; }

			private:
				static constexpr uint8_t TAG_LENGTH = 4;
				unsigned char m_Tag[TAG_LENGTH + 1]{};
				uint32_t m_CheckSum{};
				uint32_t m_Offset{};
				uint32_t m_Length{};
			};
		}
	}
}
