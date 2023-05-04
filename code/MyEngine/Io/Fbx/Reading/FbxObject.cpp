#include "pch.h"
#include "FbxObject.h"
#include <Io/Binary/LittleEndianReader.h>

using namespace Io::Binary;

Io::Fbx::Reading::FbxObject::FbxObject(std::istream& stream, uint8_t version, bool isRoot)
{
	if (isRoot)
	{
		const std::streampos begin{ stream.tellg() };
		stream.seekg(0, std::ios_base::end);
		m_End = stream.tellg();
		stream.seekg(begin);
		m_NrProps = 0;
		m_PropLength = 0;
		m_Name = "Root";
	}
	else
		ReadNode(stream, version);

	std::streampos cur = stream.tellg();
	while (cur < m_End)
	{
		if (stream.peek() == '\0')
		{
			const uint8_t nrNullsAtEnd = version == 75 ? 25 : 13;
			const auto tillEnd = m_End - cur;
			if(tillEnd== nrNullsAtEnd)
			{
				stream.seekg(m_End);
				return;
			}
			if(isRoot)
			{
				char chars[25];
				stream.get(chars, nrNullsAtEnd);
				stream.seekg(cur);
				for (int i = 0; i < nrNullsAtEnd; i++)
					if (chars[i] != '\0')
						break;

				stream.seekg(m_End);
				return;
			}

		}

		m_Children.push_back(new FbxObject(stream, version));
		cur = stream.tellg();
	}
}

Io::Fbx::Reading::FbxObject::~FbxObject()
{
	for (auto pObject : m_Children)
		delete pObject;
	for (auto pProp : m_Properties)
		delete pProp;
}

void Io::Fbx::Reading::FbxObject::Print(bool compact, int nrTabs) const
{
	for (int i = 0; i < nrTabs; i++)
		std::cout << '\t';
	std::cout << m_Name << std::endl;
	for (int i = 0; i < m_Properties.size(); i++)
		m_Properties[i]->Print(compact, nrTabs);
	if (m_Name == "Root")
		nrTabs--;
	for (int i = 0; i < m_Children.size(); i++)
		m_Children[i]->Print(compact, nrTabs + 1);
}

const Io::Fbx::Reading::FbxObject* Io::Fbx::Reading::FbxObject::GetChild(const std::string& name) const
{
	for (int i = 0; i < m_Children.size(); i++)
		if (m_Children[i]->GetName() == name)
			return m_Children[i];
	Logger::PrintError("FbxChild with name " + name + " not found");
	return nullptr;
}

const Io::Fbx::Reading::FbxObject& Io::Fbx::Reading::FbxObject::GetChild(int idx) const
{
	return *m_Children[idx];
}

std::vector<Io::Fbx::Reading::FbxObject*> Io::Fbx::Reading::FbxObject::GetChildren(const std::string& name) const
{
	std::vector<FbxObject*> result{};
	for (int i = 0; i < m_Children.size(); i++)
		if (m_Children[i]->GetName() == name)
			result.push_back(m_Children[i]);
	return result;
}

const Io::Fbx::Reading::FbxProperty* Io::Fbx::Reading::FbxObject::GetProperty(int idx) const
{
	return m_Properties[idx];
}

Io::Fbx::Reading::FbxObject* Io::Fbx::Reading::FbxObject::GetChild(const std::string& name)
{
	for (int i = 0; i < m_Children.size(); i++)
		if (m_Children[i]->GetName() == name)
			return m_Children[i];
	Logger::PrintError("FbxChild with name " + name + " not found");
	return nullptr;
}

Io::Fbx::Reading::FbxObject& Io::Fbx::Reading::FbxObject::GetChild(int idx)
{
	return *m_Children[idx];
}

Io::Fbx::Reading::FbxProperty* Io::Fbx::Reading::FbxObject::GetProperty(int idx)
{
	return m_Properties[idx];
}

void Io::Fbx::Reading::FbxObject::ReadNode(std::istream& stream, uint8_t version)
{
	const LittleEndianReader read{ stream };
	if(version == 74 || version == 73)
	{
		m_End = read.Uint32();
		m_NrProps = read.Uint32();
		m_PropLength = read.Uint32();
	}
	else if(version >= 75)
	{
		m_End = read.Uint64();
		m_NrProps = read.Uint64();
		m_PropLength = read.Uint64();
	}
	const uint8_t nameLength = read.Uint8();
	m_Name = read.String(nameLength);
	const int begin = stream.tellg();

	for (int i = 0; i < m_NrProps; i++)
	{
		auto pProp = FbxProperty::Read(stream);
		m_Properties.push_back(pProp);
	}

	//stream.seekg(begin + m_PropLength, std::ios_base::beg);
}
