#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Binary
		{
			class BitStream
			{
			public:
				explicit BitStream(std::istream& stream);

				template<typename T = uint8_t>
				T ReadBits(uint8_t nrBits);

			private:
				std::istream& m_Stream;
				uint8_t m_CurrByte{};
				uint8_t m_CurrBitOffset;
			};

			template <typename T>
			T BitStream::ReadBits(uint8_t nrBits)
			{
				if (nrBits > sizeof(T) * 8)
				{
					Logger::PrintError("NrBits is too big for this return type");
					return 0;
				}
				T result = 0;
				for (int iBit = nrBits - 1; iBit >= 0; iBit--)
				{
					result += (m_CurrByte & 1) << iBit;

					if (m_CurrBitOffset == 7)
					{
						m_CurrBitOffset = 0;
						m_CurrByte = m_Stream.get();
					}
					else
					{
						m_CurrBitOffset++;
						m_CurrByte >>= 1;
					}
				}
				return result;
			}
		}
	}
}
