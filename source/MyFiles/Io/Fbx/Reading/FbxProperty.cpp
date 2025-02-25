#include "FbxProperty.h"
#include <Io/Binary/LittleEndianReader.h>
#include <Io/Fbx/Reading/Properties/FbxPropPrimitive.h>
#include <Io/Fbx/Reading/Properties/FbxPropArray.h>
#include <Io/Fbx/Reading/Properties/FbxPropString.h>
#include <Io/Fbx/Reading/Properties/FbxPropRaw.h>

#include "Logger/Logger.h"

using namespace MyEngine::Io::Binary;

MyEngine::Io::Fbx::Reading::FbxProperty* MyEngine::Io::Fbx::Reading::FbxProperty::Read(std::istream& stream)
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
		Logger::Error("Unknown property type in fbx: " + std::string{ c });
		return nullptr;
	}
}

const MyEngine::Io::Fbx::Reading::FbxPropRaw& MyEngine::Io::Fbx::Reading::FbxProperty::AsRaw() const
{
	return *reinterpret_cast<const FbxPropRaw*>(this);
}

const MyEngine::Io::Fbx::Reading::FbxPropString& MyEngine::Io::Fbx::Reading::FbxProperty::AsStringProp() const
{
	return *reinterpret_cast<const FbxPropString*>(this);
}

const std::string& MyEngine::Io::Fbx::Reading::FbxProperty::AsString() const
{
	return reinterpret_cast<const FbxPropString*>(this)->GetValue();
}

MyEngine::Io::Fbx::Reading::FbxPropRaw& MyEngine::Io::Fbx::Reading::FbxProperty::AsRaw()
{
	return *reinterpret_cast<FbxPropRaw*>(this);
}

MyEngine::Io::Fbx::Reading::FbxPropString& MyEngine::Io::Fbx::Reading::FbxProperty::AsStringProp()
{
	return *reinterpret_cast<FbxPropString*>(this);
}

std::string& MyEngine::Io::Fbx::Reading::FbxProperty::AsString()
{
	return reinterpret_cast<FbxPropString*>(this)->GetValue();
}

void MyEngine::Io::Fbx::Reading::FbxProperty::BeginPrint(int nrTabs) const
{
	for (int i = 0; i < nrTabs; i++)
		std::cout << '\t';
	std::cout << "- ";
}
