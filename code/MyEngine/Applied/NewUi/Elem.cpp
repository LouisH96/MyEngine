#include "pch.h"
#include "Elem.h"

using namespace NewUi;

Float2 ResizePref::GetPreferredSize() const
{
	Float2 size{ maxSize };

	if (horMode == Min) size.x = minSize.x;
	if (verMode == Min) size.y = minSize.y;

	return size;
}

void Elem::UpdateTreePositions(const Float2& position)
{
	m_Bounds.Move(position);
}

Elem* Elem::GetElemAt(const Float2& position)
{
	return GetBounds().ContainsPoint(position)
		? this : nullptr;
}

void Elem::SetSize(const Float2& size)
{
	m_Bounds.SetSize(size);
}

void Elem::SetSize(const ResizePref& pref)
{
	SetSize(pref.GetPreferredSize());
}

void Elem::SetRelativePosition(const Float2& relativePosition)
{
	m_Bounds.SetLeftBot(relativePosition);
}

void Elem::RecreateTree()
{
	ClearTree();
	CreateTree();
}

void Elem::ClearTree()
{
	Clear();
}

void Elem::CreateTree()
{
	Create();
}
