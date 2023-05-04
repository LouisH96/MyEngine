#include "pch.h"
#include "FbxPropRaw.h"
#include <Io/Binary/LittleEndianReader.h>

using namespace Io::Binary;

MyEngine::Io::Fbx::Reading::FbxPropRaw::FbxPropRaw(std::istream& stream)
{
	const unsigned size{ LittleEndianReader::Uint32(stream) };
	stream.ignore(size);
}

void MyEngine::Io::Fbx::Reading::FbxPropRaw::Print(bool compact, int nrTabs) const
{
	BeginPrint(nrTabs);
	std::cout << "raw\n";
}
