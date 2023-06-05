#include "pch.h"
#include "BitStream.h"

Io::Binary::BitStream::BitStream(std::istream& stream)
	: m_Stream(stream)
	, m_CurrByte(stream.get())
	, m_CurrBitOffset(0)
{
}

void Io::Binary::BitStream::ToNextByte()
{
	m_CurrBitOffset = 0;
	m_CurrByte = m_Stream.get();
}
