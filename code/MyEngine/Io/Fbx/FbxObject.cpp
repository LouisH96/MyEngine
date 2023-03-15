#include "pch.h"
#include "FbxObject.h"
#include <Io\Binary\Bini.h>

using namespace Io::Binary;

MyEngine::Io::Fbx::FbxObject::FbxObject(std::istream& stream)
{
	ReadNode(stream);

	std::streampos cur = stream.tellg();
	while (cur < m_End)
	{
		if (stream.peek() == '\0')
		{
			stream.seekg(m_End);
			return;
		}

		m_Children.push_back(new FbxObject(stream));
		cur = stream.tellg();
	}
}

MyEngine::Io::Fbx::FbxObject::~FbxObject()
{
	for (auto pObject : m_Children)
		delete pObject;
	for (auto pProp : m_Properties)
		delete pProp;
}

void MyEngine::Io::Fbx::FbxObject::Print(int nrTabs)
{
	for (int i = 0; i < nrTabs; i++)
		std::cout << '\t';
	std::cout << m_Name << std::endl;
	for (int i = 0; i < m_Properties.size(); i++)
		m_Properties[i]->Print(nrTabs);
	for (int i = 0; i < m_Children.size(); i++)
		m_Children[i]->Print(nrTabs + 1);
}

void MyEngine::Io::Fbx::FbxObject::ReadNode(std::istream& stream)
{
	Bini read{ stream };
	m_End = read.Uint32();
	m_NrProps = read.Uint32();
	m_PropLength = read.Uint32();
	const uint8_t nameLength = read.Uint8();
	m_Name = read.String(nameLength);
	const int begin =stream.tellg();

	for (int i = 0; i < m_NrProps; i++)
	{
		auto pProp = FbxProperty::Read(stream);
		m_Properties.push_back(pProp);
	}
	stream.seekg(begin + m_PropLength, std::ios_base::beg);
}
