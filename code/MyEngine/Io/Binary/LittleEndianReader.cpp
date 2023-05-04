#include "pch.h"
#include "LittleEndianReader.h"

MyEngine::Io::Binary::LittleEndianReader::LittleEndianReader(std::istream& stream)
	: m_Stream(stream)
{
}

uint64_t Io::Binary::LittleEndianReader::Uint64() const
{
	return Uint64(m_Stream);
}

unsigned int MyEngine::Io::Binary::LittleEndianReader::Uint32() const
{
	return Uint32(m_Stream);
}

uint16_t Io::Binary::LittleEndianReader::Uint16() const
{
	return Uint16(m_Stream);
}

uint16_t Io::Binary::LittleEndianReader::Uint16(std::istream& stream)
{
	uint16_t result = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[1] = static_cast<unsigned char>(stream.get());
	return result;
}

uint8_t MyEngine::Io::Binary::LittleEndianReader::Uint8() const
{
	return Uint8(m_Stream);
}

std::string MyEngine::Io::Binary::LittleEndianReader::String(int size) const
{
	return String(m_Stream, size);
}

uint64_t Io::Binary::LittleEndianReader::Uint64(std::istream& stream)
{
	uint64_t result = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[1] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[2] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[3] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[4] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[5] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[6] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[7] = static_cast<unsigned char>(stream.get());
	return result;
}

unsigned int MyEngine::Io::Binary::LittleEndianReader::Uint32(std::istream& stream)
{
	unsigned int result = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[1] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[2] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[3] = static_cast<unsigned char>(stream.get());
	return result;
}

uint8_t MyEngine::Io::Binary::LittleEndianReader::Uint8(std::istream& stream)
{
	return static_cast<unsigned char>(stream.get());
}

std::string MyEngine::Io::Binary::LittleEndianReader::String(std::istream& stream, int size)
{
	std::string string(size, ' ');
	stream.read(&string[0], size);
	return string;
}
