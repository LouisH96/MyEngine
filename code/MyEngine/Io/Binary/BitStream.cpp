#include "pch.h"
#include "BitStream.h"

Io::Binary::BitStream::BitStream(std::istream& stream)
	: m_Stream(stream)
	, m_CurrByte(stream.get())
	, m_CurrBitOffset(0)
{
}
