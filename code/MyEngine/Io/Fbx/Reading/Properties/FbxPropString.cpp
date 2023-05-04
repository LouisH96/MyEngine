#include "pch.h"
#include "FbxPropString.h"
#include <Io\Binary\LittleEndianReader.h>

using namespace Io::Binary;

MyEngine::Io::Fbx::Reading:: FbxPropString::FbxPropString(std::istream& stream)
    : m_String(Read(stream))
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
    std::string string( size, ' ' );
    stream.read(&string[0], size);
    return string;
}
