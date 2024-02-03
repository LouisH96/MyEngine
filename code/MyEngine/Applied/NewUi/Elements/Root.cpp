#include "pch.h"
#include "Root.h"

using namespace NewUi;

Root::Root(const Float2& initSize)
{
	SetSize(initSize);
	SetRelativePosition({});
}

void Root::UpdateRootSize(const Float2& size)
{
	SetSize(size);
	ClearTree();
	CreateUi();
}

void Root::AddChild(Elem* pChild)
{
	m_Children.Add({ pChild });
}

void Root::CreateUi()
{
	//Calculate sizes
	const Float2 margin{ 10, 10 };

	ResizePref pref;
	pref.maxSize = GetSize() - margin * 2;
	pref.minSize = {};
	pref.horMode = Max;
	pref.verMode = Max;

	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		Elem& child{ GetChild(i) };
		UpdateChildSize(i, pref);
		SetChildPosition(i, {});
		child.UpdateTreePositions(GetPosition() + margin);
	}

	//Create
	CreateTree();
}

const std::string Root::GetTypeName() const
{
	return "Root";
}

void Root::RemoveChildren()
{
	for (unsigned i = 0; i < m_Children.GetSize(); i++)
	{
		m_Children[i].pChild->ClearTree();
		delete m_Children[i].pChild;
	}
	m_Children.Clear();
}

void Root::Clear()
{}

void Root::Create()
{}

void Root::UpdateSizeAndTreePositions(const ResizePref&)
{}
