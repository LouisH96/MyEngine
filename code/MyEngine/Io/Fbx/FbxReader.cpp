#include "pch.h"
#include "FbxReader.h"
#include <Io\Fbx\FbxObject.h>

//https://code.blender.org/2013/08/fbx-binary-file-format-specification/


Io::Fbx::FbxReader::FbxReader(const std::wstring& path)
	: m_Stream{ path, std::ifstream::binary }
{
	m_Stream.seekg(0, std::ios_base::end);
	const auto end = m_Stream.tellg();
	m_Stream.seekg(0, std::ios_base::beg);

	ReadHeader();

	bool stop = false;
	std::vector< FbxObject*> objects{};
	while (!stop)
	{
		auto pObject = new FbxObject(m_Stream);
		objects.push_back(pObject);
		const auto cur = m_Stream.tellg();
		if (pObject->GetEnd() == 0)
			stop = true;
		pObject->Print();
	}

	for(auto pObject : objects)
		delete pObject;

	std::cout << "finished reading header\n";
}

void Io::Fbx::FbxReader::ReadHeader()
{
	//file type
	constexpr auto fileTypeSize = 21 + 1;
	char fileType[fileTypeSize];
	m_Stream.get(fileType, fileTypeSize);

	//mystery bytes
	constexpr auto mysteryBytesSize = 2 + 1;
	char mysteryBytes[mysteryBytesSize];
	m_Stream.get(mysteryBytes, mysteryBytesSize);

	//version number
	const unsigned versionNumber{ ReadUnsignedInt() };
}

void Io::Fbx::FbxReader::ReadNode()
{
	const auto endOffset{ ReadUnsignedInt() };
	if (endOffset == 0)
	{
		m_AtEnd = true;
		return;
	}
	const auto numProperties{ ReadUnsignedInt() };
	const auto propertyListLen{ ReadUnsignedInt() };
	const int8_t nameLength{ static_cast<int8_t>(m_Stream.get()) };
	char* pName = new char[nameLength + 1];
	m_Stream.get(pName, nameLength + 1);
	const std::string name{ pName };
	delete[] pName;
	std::cout << "(" << name << ") ";
	std::cout << "nrProp: " << numProperties;
	std::cout << ", propLength: " << propertyListLen;
	std::cout << std::endl;

	if (numProperties == 0)
		ReadNode();
	else

		m_Stream.seekg(endOffset, std::ios_base::beg);
}

unsigned Io::Fbx::FbxReader::ReadUnsignedInt()
{
	return static_cast<unsigned char>(m_Stream.get())
		| static_cast<unsigned char>(m_Stream.get()) << 8
		| static_cast<unsigned char>(m_Stream.get()) << 8 * 2
		| static_cast<unsigned char>(m_Stream.get()) << 8 * 3;
}

unsigned Io::Fbx::FbxReader::ToUnsignedInt(const char* pBytes)
{
	return static_cast<unsigned char>(pBytes[3]) << 8 * 3
		| static_cast<unsigned char>(pBytes[2]) << 8 * 2
		| static_cast<unsigned char>(pBytes[1]) << 8 * 1
		| static_cast<unsigned char>(pBytes[0]);
}

