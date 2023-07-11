#include "BinaryFbxReader.h"

#include <fstream>

#include "FbxFile.h"
#include "FbxProperty.h"
#include "Io/Binary/LittleEndianReader.h"
#include "Logger/Logger.h"

using namespace MyEngine;
using namespace Io;
using namespace Binary;
using namespace Fbx::Reading;

FbxFile BinaryFbxReader::Read(const std::wstring& path)
{
	//create output
	FbxFile file{};
	file.SetIsBinary(true);

	std::ifstream stream{path, std::ifstream::binary};
	if (!stream.is_open())
	{
		Logger::PrintError("[BinaryFbxReader] could not open file");
		return file;
	}

	//read header
	const std::string fileType{LittleEndianReader::String(stream, 21) };
	const uint16_t mystery{ LittleEndianReader::Uint16(stream) }; //should be 26
	const unsigned versionNumber{ LittleEndianReader::Uint32(stream) };
	file.SetVersion(versionNumber);

	//create root-element
	file.GetRoot().SetName("Root");
	const std::streampos begin{stream.tellg()};
	stream.seekg(0, std::ios_base::end);
	const ElementInfo rootInfo{ stream.tellg(), 0, 0 };
	stream.seekg(begin);

	//start reading elements
	ReadChildren(stream, file.GetRoot(), rootInfo, file);

	return file;
}

BinaryFbxReader::ElementInfo BinaryFbxReader::ReadElementInfo(std::istream& stream, const FbxFile& file)
{
	ElementInfo info{};
	if (file.GetVersion() >= 7500)
	{
		info.End = LittleEndianReader::Uint64(stream); //todo
		info.NrProperties = LittleEndianReader::Uint64(stream);
		info.PropertiesLength = LittleEndianReader::Uint64(stream);
	}
	else
	{
		info.End = LittleEndianReader::Uint32(stream);
		info.NrProperties = LittleEndianReader::Uint32(stream);
		info.PropertiesLength = LittleEndianReader::Uint32(stream);
	}
	return info;
}

void BinaryFbxReader::ReadProperties(std::istream& stream, FbxElement& parent, const ElementInfo& info,
	const FbxFile& file)
{
	for (unsigned i = 0; i < info.NrProperties; i++)
		parent.AddProperty(FbxProperty::Read(stream));
}

void BinaryFbxReader::ReadChildren(std::istream& stream, FbxElement& parent, const ElementInfo& info,
	const FbxFile& file)
{
	while (stream.tellg() < info.End)
	{
		if (stream.peek() == '\0')
		{
			const uint8_t padding{ static_cast<uint8_t>(file.GetVersion() >= 7500 ? 25 : 13) };
			if (info.End - stream.tellg() == padding)
			{
				stream.seekg(info.End);
				return;
			}
			if (parent.GetName() == "Root")
			{
				stream.seekg(info.End);
				return;
			}
			auto diff{ info.End - stream.tellg() };
		}
		FbxElement& child{ parent.CreateChild() };
		const ElementInfo childInfo{ ReadElementInfo(stream, file) };
		const uint8_t nameLength = LittleEndianReader::Uint8(stream);
		child.SetName(LittleEndianReader::String(stream, nameLength));
		ReadProperties(stream, child, childInfo, file);
		ReadChildren(stream, child, childInfo, file);
	}
}
