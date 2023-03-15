#include "pch.h"
#include "FbxPropRaw.h"
#include <Io\Binary\Bini.h>

using namespace Io::Binary;

MyEngine::Io::Fbx::FbxPropRaw::FbxPropRaw(std::istream& stream)
{
	const unsigned size{ Bini::Uint32(stream) };
	stream.ignore(size);
}

void MyEngine::Io::Fbx::FbxPropRaw::Print(int nrTabs) const
{
	BeginPrint(nrTabs);
	std::cout << "raw\n";
}
