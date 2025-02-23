#include "pch.h"
#include "Elem.h"

#include "UiSystem.h"

using namespace Ui;

void ResizePref::SetMin()
{
	horMode = FillMode::Min;
	verMode = FillMode::Min;
}

void ResizePref::SetMax()
{
	horMode = FillMode::Max;
	verMode = FillMode::Max;
}

Float2 ResizePref::GetPreferredSize() const
{
	Float2 size{ maxSize };

	if (horMode == FillMode::Min) size.x = minSize.x;
	if (verMode == FillMode::Min) size.y = minSize.y;

	return size;
}

void Elem::UpdateTreePositions(const Float2& position)
{
	m_Bounds.Move(position);
}

Elem::Elem(bool takeMouse)
	: m_TakeMouse{ takeMouse }
{
}

const Float2 Elem::GetCenter() const
{
	return GetSize() * .5f;
}

Elem* Elem::GetElemAt(const Float2& position)
{
	return m_TakeMouse && GetBounds().ContainsPoint(position)
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

void Elem::SetWidth(float width)
{
	m_Bounds.SetWidth(width);
}

void Elem::SetHeight(float height)
{
	m_Bounds.SetHeight(height);
}

void Elem::RequestUpdate()
{
	UI_TREE.RequestUpdate();
}

void Elem::AssertWithinMaxSize(const Float2& desired, const ResizePref& pref)
{
	if (desired.x > pref.maxSize.x)
		Logger::PrintWarning("[Elem::AssertWithinMaxSize] Elem is too wide for parent");
	if (desired.y > pref.maxSize.y)
		Logger::PrintWarning("[Elem::AssertWithinMaxSize] Elem is too high for parent");
}

UiShapeRenderer& Elem::Draw()
{
	return UI.GetShapeRenderer();
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
