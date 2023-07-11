#include "FbxElement.h"

#include "FbxProperty.h"
#include "Logger/Logger.h"

MyEngine::Io::Fbx::Reading::FbxElement::FbxElement(std::string name)
	: m_Name{std::move(name)}
{
}

MyEngine::Io::Fbx::Reading::FbxElement::~FbxElement()
{
	m_Properties.DeleteAll();
}

MyEngine::Io::Fbx::Reading::FbxElement::FbxElement(FbxElement&& other) noexcept
	: m_Name{ std::move(other.m_Name) }
	, m_Children{ std::move(other.m_Children) }
	, m_Properties{ std::move(other.m_Properties) }
{
}

MyEngine::Io::Fbx::Reading::FbxElement& MyEngine::Io::Fbx::Reading::FbxElement::operator=(FbxElement&& other) noexcept
{
	m_Name = std::move(other.m_Name);
	m_Children = std::move(other.m_Children);
	m_Properties = std::move(other.m_Properties);
	return *this;
}

void MyEngine::Io::Fbx::Reading::FbxElement::SetName(std::string name)
{
	m_Name = std::move(name);
}

void MyEngine::Io::Fbx::Reading::FbxElement::AddProperty(FbxProperty* pProperty)
{
	m_Properties.Add(pProperty);
}

void MyEngine::Io::Fbx::Reading::FbxElement::EnsurePropertyCapacity(unsigned minCapacity)
{
	m_Properties.EnsureCapacity(minCapacity);
}

MyEngine::Io::Fbx::Reading::FbxElement& MyEngine::Io::Fbx::Reading::FbxElement::CreateChild()
{
	m_Children.Add({});
	return m_Children.Last();
}
