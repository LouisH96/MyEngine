#include "FbxElement.h"

#include "FbxProperty.h"
#include "Logger/Logger.h"

using namespace MyEngine::Io::Fbx::Reading;

FbxElement::FbxElement(std::string name)
	: m_Name{ std::move(name) }
{
}

FbxElement::~FbxElement()
{
	m_Properties.DeleteAll();
}

FbxElement::FbxElement(FbxElement&& other) noexcept
	: m_Name{ std::move(other.m_Name) }
	, m_Children{ std::move(other.m_Children) }
	, m_Properties{ std::move(other.m_Properties) }
{
}

FbxElement& FbxElement::operator=(FbxElement&& other) noexcept
{
	m_Name = std::move(other.m_Name);
	m_Children = std::move(other.m_Children);
	m_Properties = std::move(other.m_Properties);
	return *this;
}

void FbxElement::SetName(std::string name)
{
	m_Name = std::move(name);
}

void FbxElement::AddProperty(FbxProperty* pProperty)
{
	m_Properties.Add(pProperty);
}

void FbxElement::EnsurePropertyCapacity(unsigned minCapacity)
{
	m_Properties.EnsureCapacity(minCapacity);
}

FbxElement& FbxElement::CreateChild()
{
	m_Children.Add(FbxElement{});
	return m_Children.Last();
}

MyEngine::List<const FbxElement*> FbxElement::GetChildren(const std::string& name) const
{
	List<const FbxElement*> children{};
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		if (m_Children[i].GetName() == name)
			children.Add(&m_Children[i]);;
	return children;
}

const FbxElement& FbxElement::GetChild(unsigned idx) const
{
	return m_Children[idx];
}

const FbxElement* FbxElement::GetChild(const std::string& name) const
{
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		if (m_Children[i].GetName() == name)
			return &m_Children[i];
	Logger::Error("[FbxElement] Child with name " + name + " not found");
	return nullptr;
}

MyEngine::List<FbxElement*> FbxElement::GetChildren(const std::string& name)
{
	List<FbxElement*> children{};
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		if (m_Children[i].GetName() == name)
			children.Add(&m_Children[i]);;
	return children;
}

FbxElement& FbxElement::GetChild(unsigned idx)
{
	return m_Children[idx];
}

FbxElement* FbxElement::GetChild(const std::string& name)
{
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		if (m_Children[i].GetName() == name)
			return &m_Children[i];
	Logger::Error("[FbxElement] Child with name " + name + " not found");
	return nullptr;
}

const FbxProperty& FbxElement::GetProperty(unsigned idx) const
{
	return *m_Properties[idx];
}

const FbxProperty& FbxElement::GetChildProperty(unsigned childIdx, unsigned propertyIdx) const
{
	return m_Children[childIdx].GetProperty(propertyIdx);
}

const FbxProperty& FbxElement::GetLastProperty() const
{
	return *m_Properties.Last();
}

FbxProperty& FbxElement::GetProperty(unsigned idx)
{
	return *m_Properties[idx];
}

FbxProperty& FbxElement::GetChildProperty(unsigned childIdx, unsigned propertyIdx)
{
	return m_Children[childIdx].GetProperty(propertyIdx);
}

FbxProperty& FbxElement::GetLastProperty()
{
	return *m_Properties.Last();
}

void FbxElement::Print(bool compact, unsigned nrTabs) const
{
	for (unsigned i = 0; i < nrTabs; i++)
		std::cout << '\t';
	std::cout << m_Name << std::endl;
	for (unsigned i = 0; i < m_Properties.GetSize(); i++)
		m_Properties[i]->Print(compact, nrTabs);
	if (m_Name == "Root")
		nrTabs--;
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
		m_Children[i].Print(compact, nrTabs + 1);
}
