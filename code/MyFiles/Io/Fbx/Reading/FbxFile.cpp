#include "FbxFile.h"

#include "Logger/Logger.h"

void MyEngine::Io::Fbx::Reading::FbxFile::SetVersion(uint32_t version)
{
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
