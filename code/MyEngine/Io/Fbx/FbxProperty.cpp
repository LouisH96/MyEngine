#include "pch.h"
#include "FbxProperty.h"
#include <Io/Binary/Bini.h>
#include <Io\Fbx\Properties\FbxPropPrimitive.h>
#include <Io\Fbx\Properties\FbxPropArray.h>
#include <Io\Fbx\Properties\FbxPropString.h>
#include <Io\Fbx\Properties\FbxPropRaw.h>

using namespace Io::Binary;

MyEngine::Io::Fbx::FbxProperty* MyEngine::Io::Fbx::FbxProperty::Read(std::istream& stream)
{
	const char c = stream.get();
	switch (c)
	{
	case 'Y': return new FbxPropPrimitive<uint16_t>(stream);
	case 'C': return new FbxPropPrimitive<bool>(stream);
	case 'I': return new FbxPropPrimitive<uint32_t>(stream);
	case 'F': return new FbxPropPrimitive<float>(stream);
	case 'D': return new FbxPropPrimitive<double>(stream);
	case 'L': return new FbxPropPrimitive<uint64_t>(stream);
	case 'f': return new FbxPropArray<float>(stream);
	case 'd': return new FbxPropArray<double>(stream);
	case 'l': return new FbxPropArray<uint64_t>(stream);
	case 'i': return new FbxPropArray<uint32_t>(stream);
	case 'b': return new FbxPropArray<bool>(stream);
	case 'S': return new FbxPropString(stream);
	case 'R': return new FbxPropRaw(stream);
	default:
		Logger::PrintError("Unknown property type in fbx: " + std::string{ c });
		return nullptr;
	}
}

void MyEngine::Io::Fbx::FbxProperty::BeginPrint(int nrTabs)
{
	for (int i = 0; i < nrTabs; i++)
		std::cout << '\t';
	std::cout << "- ";
}