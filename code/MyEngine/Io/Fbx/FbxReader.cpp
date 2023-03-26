#include "pch.h"
#include "FbxReader.h"
#include <Io\Fbx\FbxObject.h>
#include <Io\Binary\Bini.h>

//https://code.blender.org/2013/08/fbx-binary-file-format-specification/


Io::Fbx::FbxReader::FbxReader(const std::wstring& path)
	: m_Stream{ path, std::ifstream::binary }
{
	if (!m_Stream.is_open())
	{
		Logger::PrintError("Could not open Fbx file");
		return;
	}

	m_Stream.seekg(0, std::ios_base::end);
	const auto end = m_Stream.tellg();
	m_Stream.seekg(0, std::ios_base::beg);
	const uint8_t version = ReadHeader();
	m_pRoot = new FbxObject(m_Stream, version, true);
}

Io::Fbx::FbxReader::~FbxReader()
{
	delete m_pRoot;
}

uint8_t Io::Fbx::FbxReader::ReadHeader()
{
	using namespace Binary;

	//file type
	const std::string fileType{ Bini::String(m_Stream, 21) };

	//mystery bytes
	const uint16_t mystery{ Bini::Uint16(m_Stream) };

	//version number
	const unsigned versionNumber{ Bini::Uint32(m_Stream) };
	if (versionNumber == 7400)
		return 74;
	if (versionNumber == 7500)
		return 75;
	Logger::PrintError("FbxVersion " + std::to_string(versionNumber) + " not supported");
	return 0;
}

unsigned Io::Fbx::FbxReader::ReadUnsignedInt()
{
	return static_cast<unsigned char>(m_Stream.get())
		| static_cast<unsigned char>(m_Stream.get()) << 8
		| static_cast<unsigned char>(m_Stream.get()) << 8 * 2
		| static_cast<unsigned char>(m_Stream.get()) << 8 * 3;
}

unsigned Io::Fbx::FbxReader::ToUnsignedInt(const char* pBytes)
{
	return static_cast<unsigned char>(pBytes[3]) << 8 * 3
		| static_cast<unsigned char>(pBytes[2]) << 8 * 2
		| static_cast<unsigned char>(pBytes[1]) << 8 * 1
		| static_cast<unsigned char>(pBytes[0]);
}

