#include "FbxReader.h"

#include "BinaryFbxReader.h"
#include "FbxFile.h"

MyEngine::Io::Fbx::Reading::FbxFile MyEngine::Io::Fbx::Reading::FbxReader::Read(const std::wstring& path)
{
	return BinaryFbxReader::Read(path);
}
