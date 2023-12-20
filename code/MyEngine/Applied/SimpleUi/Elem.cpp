#include "pch.h"
#include "Elem.h"

using namespace SimpleUi;

Elem::Elem(const RectFloat& bounds)
	: m_Bounds{ bounds }
{
}

Elem::~Elem()
{
	m_pParent = nullptr;
	m_pSibling = nullptr;

	const Elem* pChild{ m_pChild };
	while (pChild)
	{
		const Elem* pNext{ pChild->m_pSibling };
		delete m_pChild;
		pChild = pNext;
	}

	m_pChild = nullptr;
}

void Elem::SetBounds(const RectFloat& newBounds)
{
	const RectFloat oldBounds{ m_Bounds };
	m_Bounds = newBounds;
	OnBoundsChanged(oldBounds);
	Remake();
}

void Elem::SetLeftBot(const Float2& newPos)
{
	SetBounds({ newPos, m_Bounds.GetSize() });
}

void Elem::SetSize(const Float2& newSize)
{
	SetBounds({ m_Bounds.GetLeftBot(), newSize });
}

void Elem::Move(const Float2& displacement)
{
	SetBounds({ m_Bounds.GetLeftBot() + displacement, m_Bounds.GetSize() });
}

void Elem::AddChild(Elem* pChild)
{
	pChild->LeaveFamily();

	if (m_pChild)
		GetLastChild()->m_pSibling = pChild;
	else
		m_pChild = pChild;

	pChild->m_pParent = this;

	OnChildAdded(*pChild);
}

void Elem::Remake()
{
	if (IsAlive())
		Clear();
	Make();
}

void Elem::LeaveFamily()
{
	if (!m_pParent)
		return;

	Elem* pPreviousSibling{ m_pParent->GetChildBefore(*this) };
	if (pPreviousSibling)
		pPreviousSibling->m_pSibling = m_pSibling;

	m_pParent = nullptr;
	m_pSibling = nullptr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
Elem* Elem::GetChildBefore(const Elem& child)
{
	if (m_pChild == &child)
		return nullptr;

	Elem* pPrevious{ m_pChild->m_pSibling };
	while (pPrevious->m_pSibling)
	{
		if (pPrevious->m_pSibling == &child)
			return pPrevious;
		pPrevious = pPrevious->m_pSibling;
	}

	Logger::PrintError("[Elem::GetChildBefore] element is not a child of this parent");
	return nullptr;
}

// ReSharper disable once CppMemberFunctionMayBeConst
Elem* Elem::GetLastChild()
{
	Elem* pLast{ m_pChild };
	while (pLast->m_pSibling)
		pLast = m_pSibling;

	return pLast;
}
