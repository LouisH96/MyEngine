#include "pch.h"
#include "FbxPropString.h"
#include <Io\Binary\Bini.h>

using namespace Io::Binary;

MyEngine::Io::Fbx::FbxPropString::FbxPropString(std::istream& stream)
    : m_String(Read(stream))
{
}

void MyEngine::Io::Fbx::FbxPropString::Print(int nrTabs) const
{
    BeginPrint(nrTabs);
    std::cout << "[string] " << m_String << std::endl;
}

std::string MyEngine::Io::Fbx::FbxPropString::Read(std::istream& stream)
{
    const unsigned size{ Bini::Uint32(stream) };
    std::string string( size, ' ' );
    stream.read(&string[0], size);
    return string;
}
