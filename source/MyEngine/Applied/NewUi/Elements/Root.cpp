#include "pch.h"
#include "Root.h"

#include <App\ResizedEvent.h>

using namespace NewUi;

Root::Root(const Float2& initSize)
{
	SetSize(initSize);
}

void Root::AddChild(Elem* pChild)
{
	ParentElem::AddChild({ pChild });
}

const std::string Root::GetTypeName() const
{
	return "Root";
}

void Root::Create()
{
}

void Root::Clear()
{
}

void Root::UpdateSizeAndTreePositions(const ResizePref& resizePref)
{
	for (unsigned i = 0; i < GetNrChildren(); i++)
	{
		UpdateChildSize(i, resizePref);
		SetChildPosition(i, { 0,0 });
	}
}
