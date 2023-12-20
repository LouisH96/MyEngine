#include "pch.h"
#include "Root.h"

using namespace SimpleUi;

Root::Root(const RectFloat& bounds)
	: Elem{ bounds }
{
}

void Root::Clear()
{
}

void Root::Make()
{
}

void Root::OnBoundsChanged(const RectFloat&)
{
	Logger::PrintError("[Root::OnBoundsChanged] bounds should only change if window changes");
}
