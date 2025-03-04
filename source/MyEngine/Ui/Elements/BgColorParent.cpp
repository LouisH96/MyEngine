#include "pch.h"
#include "BgColorParent.h"

using namespace Ui;

BgColorParent::BgColorParent(const Float3& color, bool takeMouse)
	: SingleParent{ takeMouse }
	, m_Color{ color }
{
}

void BgColorParent::TreeUpdate(const ResizePref& pref)
{
	if (GetChild())
	{
		ChildTreeUpdate(GetChild(), pref);
		ChildSetPosition(GetChild(), {});
		SetSize(GetChild()->GetSize());
	}
	else
		SetSize(pref);
}

void BgColorParent::Clear()
{
	m_BgId.Clear();
}

void BgColorParent::Create()
{
	m_BgId = UI_RECT.Add(GetBounds(), m_Color);
}
