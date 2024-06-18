#include "BigBinReader.h"

using namespace MyEngine::Io::Bin;

BigBinReader::BigBinReader(std::istream& stream)
	: m_Stream(stream)
{
}

void BigBinReader::SetPos(uint32_t offset)
{
	m_Stream.seekg(offset);
}

uint32_t BigBinReader::GetPos() const
{
	return static_cast<uint32_t>(m_Stream.tellg());
}

void BigBinReader::Read(uint64_t& destination) const
{
	Read(m_Stream, destination);
}

void BigBinReader::Read(uint32_t& destination) const
{
	Read(m_Stream, destination);
}

void BigBinReader::Read(uint16_t& destination) const
{
	Read(m_Stream, destination);
}

void BigBinReader::Read(uint8_t& destination) const
{
	Read(m_Stream, destination);
}

void BigBinReader::Read(std::istream& stream, uint64_t& destination)
{
	reinterpret_cast<uint8_t*>(&destination)[7] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[6] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[5] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[4] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[3] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[2] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[1] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[0] = static_cast<unsigned char>(stream.get());
}

void BigBinReader::Read(std::istream& stream, uint32_t& destination)
{
	reinterpret_cast<uint8_t*>(&destination)[3] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[2] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[1] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[0] = static_cast<unsigned char>(stream.get());
}

void BigBinReader::Read(std::istream& stream, uint16_t& destination)
{
	reinterpret_cast<uint8_t*>(&destination)[1] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&destination)[0] = static_cast<unsigned char>(stream.get());
}

void BigBinReader::Read(std::istream& stream, uint8_t& destination)
{
	destination = static_cast<unsigned char>(stream.get());
}

int64_t BigBinReader::Int64() const
{
	return Int64(m_Stream);
}

int32_t BigBinReader::Int32() const
{
	return Int32(m_Stream);
}

int16_t BigBinReader::Int16() const
{
	return Int16(m_Stream);
}

int8_t BigBinReader::Int8() const
{
	return Int8(m_Stream);
}

void BigBinReader::Read(int64_t& destination) const
{
	Read(m_Stream, destination);
}

void BigBinReader::Read(int32_t& destination) const
{
	Read(m_Stream, destination);
}

void BigBinReader::Read(int16_t& destination) const
{
	Read(m_Stream, destination);
}

void BigBinReader::Read(int8_t& destination) const
{
	Read(m_Stream, destination);
}

int64_t BigBinReader::Int64(std::istream& stream)
{
	int64_t result;
	Read(stream, result);
	return result;
}

int32_t BigBinReader::Int32(std::istream& stream)
{
	int32_t result;
	Read(stream, result);
	return result;
}

int16_t BigBinReader::Int16(std::istream& stream)
{
	int16_t result;
	Read(stream, result);
	return result;
}

int8_t BigBinReader::Int8(std::istream& stream)
{
	return static_cast<char>(stream.get());
}

void BigBinReader::Read(std::istream& stream, int64_t& destination)
{
	Read(stream, reinterpret_cast<uint64_t&>(destination));
}

void BigBinReader::Read(std::istream& stream, int32_t& destination)
{
	Read(stream, reinterpret_cast<uint32_t&>(destination));
}

void BigBinReader::Read(std::istream& stream, int16_t& destination)
{
	Read(stream, reinterpret_cast<uint16_t&>(destination));
}

void BigBinReader::Read(std::istream& stream, int8_t& destination)
{
	Read(stream, reinterpret_cast<uint8_t&>(destination));
}

uint64_t BigBinReader::Uint64() const
{
	return Uint64(m_Stream);
}

uint32_t BigBinReader::Uint32() const
{
	return Uint32(m_Stream);
}

uint16_t BigBinReader::Uint16() const
{
	return Uint16(m_Stream);
}

uint8_t BigBinReader::Uint8() const
{
	return Uint8(m_Stream);
}

uint64_t BigBinReader::Uint64(std::istream& stream)
{
	uint64_t result;
	reinterpret_cast<uint8_t*>(&result)[7] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[6] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[5] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[4] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[3] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[2] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[1] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[0] = static_cast<unsigned char>(stream.get());
	return result;
}

uint32_t BigBinReader::Uint32(std::istream& stream)
{
	uint32_t result;
	reinterpret_cast<uint8_t*>(&result)[3] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[2] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[1] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[0] = static_cast<unsigned char>(stream.get());
	return result;
}

uint16_t BigBinReader::Uint16(std::istream& stream)
{
	uint32_t result;
	reinterpret_cast<uint8_t*>(&result)[1] = static_cast<unsigned char>(stream.get());
	reinterpret_cast<uint8_t*>(&result)[0] = static_cast<unsigned char>(stream.get());
	return result;
}

uint8_t BigBinReader::Uint8(std::istream& stream)
{
	return static_cast<unsigned char>(stream.get());
}
