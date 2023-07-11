#include "FbxReader.h"
#include <Io/Fbx/Reading/FbxObject.h>
#include <Io/Binary/LittleEndianReader.h>

#include "Logger/Logger.h"

//https://code.blender.org/2013/08/fbx-binary-file-format-specification/

MyEngine::Io::Fbx::Reading::FbxReader::FbxReader(const std::wstring& path)
	: m_Stream{ path, std::ifstream::binary }
{
	if (!m_Stream.is_open())
	{
		Logger::PrintError("Could not open Fbx file");
		return;
	}

	const uint8_t version = ReadHeader();
	m_pRoot = new FbxObject(m_Stream, version, true);
}

MyEngine::Io::Fbx::Reading::FbxReader::~FbxReader()
{
	delete m_pRoot;
}

uint8_t MyEngine::Io::Fbx::Reading::FbxReader::ReadHeader()
{
	using namespace Binary;

	//file type
	const std::string fileType{ LittleEndianReader::String(m_Stream, 21) };

	//mystery bytes
	const uint16_t mystery{ LittleEndianReader::Uint16(m_Stream) };

	//version number
	const unsigned versionNumber{ LittleEndianReader::Uint32(m_Stream) };
	if (versionNumber == 7300) return 73;
	if (versionNumber == 7400) return 74;
	if (versionNumber == 7500) return 75;
	if (versionNumber == 7700) return 77; //https://forums.autodesk.com/t5/fbx-forum/why-does-motionbuilder-2019-export-ascii-fbx-as-7-7-0/m-p/8906055/highlight/true#M9698
	Logger::PrintError("FbxVersion " + std::to_string(versionNumber) + " not supported");
	return 0;
}

unsigned MyEngine::Io::Fbx::Reading::FbxReader::ReadUnsignedInt()
{
	return static_cast<unsigned char>(m_Stream.get())
		| static_cast<unsigned char>(m_Stream.get()) << 8
		| static_cast<unsigned char>(m_Stream.get()) << 8 * 2
		| static_cast<unsigned char>(m_Stream.get()) << 8 * 3;
}

unsigned MyEngine::Io::Fbx::Reading::FbxReader::ToUnsignedInt(const char* pBytes)
{
	return static_cast<unsigned char>(pBytes[3]) << 8 * 3
		| static_cast<unsigned char>(pBytes[2]) << 8 * 2
		| static_cast<unsigned char>(pBytes[1]) << 8 * 1
		| static_cast<unsigned char>(pBytes[0]);
}

