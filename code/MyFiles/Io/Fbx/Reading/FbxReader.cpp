#include "FbxReader.h"

#include <fstream>

#include "AsciiFbxReader.h"
#include "BinaryFbxReader.h"
#include "FbxFile.h"
#include "Logger/Logger.h"

MyEngine::Io::Fbx::Reading::FbxFile MyEngine::Io::Fbx::Reading::FbxReader::Read(const std::wstring& path)
{
	std::ifstream stream{ path, std::ifstream::binary };
	if (!stream.is_open())
	{
		Logger::PrintError("[BinaryFbxReader] could not open file");
		return {};
	}

	if (stream.peek() == ';')
		return AsciiFbxReader::Read(std::move(stream));
	else
		return BinaryFbxReader::Read(stream);
}
