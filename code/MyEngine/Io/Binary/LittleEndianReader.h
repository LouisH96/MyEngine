#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Binary
		{
			class LittleEndianReader
			{
			public:
				explicit LittleEndianReader(std::istream& stream);

				uint64_t Uint64() const;
				unsigned int Uint32() const;
				uint16_t Uint16() const;
				uint8_t Uint8() const;
				std::string String(int size) const;

				static uint64_t Uint64(std::istream& stream);
				static unsigned int Uint32(std::istream& stream);
				static uint16_t Uint16(std::istream& stream);
				static uint8_t Uint8(std::istream& stream);
				static std::string String(std::istream& stream, int size);
			private:
				std::istream& m_Stream;
			};
		}
	}
}

