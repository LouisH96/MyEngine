#include "FbxFile.h"

#include "Logger/Logger.h"

using namespace MyEngine;
using namespace Io::Fbx::Reading;

FbxFile::FbxFile(bool isBinary)
	: m_IsBinary(isBinary)
{
}

void FbxFile::SetVersion(uint32_t version)
{
	static constexpr bool printVersion{ false };
	if (printVersion)
		Logger::Print("Reading FbxVersion", version);

	if (version != 7300 &&
		version != 7400 &&
		version != 7500 &&
		version != 7700)
	{
		Logger::PrintError("[FbxFile] Unsupported FbxVersion: " + std::to_string(version));
		return;
	}
	m_Version = version;
}
