#include "pch.h"
#include "Bini.h"

MyEngine::Io::Binary::Bini::Bini(std::istream& stream)
	: m_Stream(stream)
{
}

unsigned int MyEngine::Io::Binary::Bini::Uint32() const
{
	return Uint32(m_Stream);
}

uint16_t Io::Binary::Bini::Uint16() const
{
	return Uint16(m_Stream);
}

uint16_t Io::Binary::Bini::Uint16(std::istream& stream)
{
	return static_cast<unsigned char>(stream.get())
		| static_cast<unsigned char>(stream.get()) << 8;
}

uint8_t MyEngine::Io::Binary::Bini::Uint8() const
{
	return Uint8(m_Stream);
}

std::string MyEngine::Io::Binary::Bini::String(int size) const
{
	return String(m_Stream, size);
}

unsigned int MyEngine::Io::Binary::Bini::Uint32(std::istream& stream)
{
	return static_cast<unsigned char>(stream.get())
		| static_cast<unsigned char>(stream.get()) << 8
		| static_cast<unsigned char>(stream.get()) << 8 * 2
		| static_cast<unsigned char>(stream.get()) << 8 * 3;
}

uint8_t MyEngine::Io::Binary::Bini::Uint8(std::istream& stream)
{
	return static_cast<unsigned char>(stream.get());
}

std::string MyEngine::Io::Binary::Bini::String(std::istream& stream, int size)
{
	std::string string(size, ' ');
	stream.read(&string[0], size);
	return string;
}
