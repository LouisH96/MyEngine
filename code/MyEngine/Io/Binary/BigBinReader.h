#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Bin
		{
			class BigBinReader
			{
			public:
				explicit BigBinReader(std::istream& stream);

				void SetPos(uint32_t offset) const;
				uint32_t GetPos() const;

				uint64_t Uint64() const;
				uint32_t Uint32() const;
				uint16_t Uint16() const;
				uint8_t Uint8() const;
				void Read(uint64_t& destination) const;
				void Read(uint32_t& destination) const;
				void Read(uint16_t& destination) const;
				void Read(uint8_t& destination) const;
				static uint64_t Uint64(std::istream& stream);
				static uint32_t Uint32(std::istream& stream);
				static uint16_t Uint16(std::istream& stream);
				static uint8_t Uint8(std::istream& stream);
				static void Read(std::istream& stream, uint64_t& destination);
				static void Read(std::istream& stream, uint32_t& destination);
				static void Read(std::istream& stream, uint16_t& destination);
				static void Read(std::istream& stream, uint8_t& destination);

				int64_t Int64() const;
				int32_t Int32() const;
				int16_t Int16() const;
				int8_t Int8() const;
				void Read(int64_t& destination) const;
				void Read(int32_t& destination) const;
				void Read(int16_t& destination) const;
				void Read(int8_t& destination) const;
				static int64_t Int64(std::istream& stream);
				static int32_t Int32(std::istream& stream);
				static int16_t Int16(std::istream& stream);
				static int8_t Int8(std::istream& stream);
				static void Read(std::istream& stream, int64_t& destination);
				static void Read(std::istream& stream, int32_t& destination);
				static void Read(std::istream& stream, int16_t& destination);
				static void Read(std::istream& stream, int8_t& destination);


			private:
				std::istream& m_Stream;
			};
		}
	}
}
