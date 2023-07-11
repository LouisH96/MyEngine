#include "FbxPropString.h"

#include <iostream>
#include <Io\Binary\LittleEndianReader.h>

using namespace MyEngine::Io::Binary;

MyEngine::Io::Fbx::Reading::FbxPropString::FbxPropString(std::istream& stream)
	: m_String(Read(stream))
{
}

MyEngine::Io::Fbx::Reading::FbxPropString::FbxPropString(std::string string)
	: m_String{ static_cast<std::string&&>(string) }
{
}

void MyEngine::Io::Fbx::Reading::FbxPropString::Print(bool compact, int nrTabs) const
{
	BeginPrint(nrTabs);
	std::cout << "[string] " << m_String << std::endl;
}

std::string MyEngine::Io::Fbx::Reading::FbxPropString::Read(std::istream& stream)
{
	const unsigned size{ LittleEndianReader::Uint32(stream) };
	std::string string(size, ' ');
	stream.read(&string[0], size);
	return string.substr(0, string.find('\0'));
}
