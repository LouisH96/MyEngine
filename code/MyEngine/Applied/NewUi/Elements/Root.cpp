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
	CreateTree();
}

void Root::AddChild(Elem* pChild)
{
	ParentElem::AddChild({ pChild });
}

void Root::Create()
{
	//Calculate sizes
	ResizePref pref;
	pref.maxSize = GetSize();
	pref.minSize = {};
	pref.horMode = Max;
	pref.verMode = Max;

	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		Elem& child{ GetChild(i) };
		UpdateChildSize(i, pref);
		SetChildPosition(i, {});
		child.UpdateTreePositions(GetPosition());
	}
}

const std::string Root::GetTypeName() const
{
	return "Root";
}

void Root::Clear()
{
}

void Root::UpdateSizeAndTreePositions(const ResizePref&)
{}
