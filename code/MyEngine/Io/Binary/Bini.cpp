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
	uint16_t result = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[1] = static_cast<unsigned char>(stream.get());
	return result;
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
	unsigned int result = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[1] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[2] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[3] = static_cast<unsigned char>(stream.get());
	return result;
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
