#include "pch.h"
#include "SingleParent.h"

using namespace Ui;

SingleParent::SingleParent(bool takeMouse)
	: Elem{ takeMouse }
{
}

SingleParent::~SingleParent()
{
	if (m_pChild)
	{
		delete m_pChild;
		m_pChild = nullptr;
	}
}

Elem* SingleParent::GetElemAt(const Float2& position)
{
	Elem* pElem{ nullptr };
	if (m_pChild)
	{
		pElem = m_pChild->GetElemAt(position);
		if (pElem)
			return pElem;
	}
	return Elem::GetElemAt(position);
}

void SingleParent::SetChildrenTakeMouse(bool takeMouse)
{
	if (m_pChild)
		m_pChild->SetTakeMouseAll(takeMouse);
}

Elem* SingleParent::SetChild(Elem* pChild)
{
	Elem* pOld{ m_pChild };
	m_pChild = pChild;
	RequestUpdate();
	return pOld;
}

void SingleParent::UpdateTreePositions(const Float2& position)
{
	Elem::UpdateTreePositions(position);
	if (m_pChild)
		ChildUpdateTreePositions(m_pChild, GetPosition());
}

void SingleParent::ClearTree()
{
	if (m_pChild)
		ChildClearTree(m_pChild);
	Clear();
}

void SingleParent::CreateTree()
{
	Create();
	if (m_pChild)
		ChildCreateTree(m_pChild);
}
