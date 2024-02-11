#include "pch.h"
#include "SideMenu.h"

#include "SideMenuPath.h"
#include "Applied/NewUi/NewUiSystem.h"
#include "Applied/NewUi/Elements/AnchorParent.h"
#include "Applied/NewUi/Elements/Border.h"
#include "Applied/NewUi/Elements/Box.h"
#include "Applied/NewUi/Elements/Extender.h"
#include "Applied/NewUi/Elements/Label.h"
#include "Applied/NewUi/Elements/ListElem.h"
#include "Applied/NewUi/Elements/Margin.h"
#include "Gui/GuiRenderer.h"

using namespace NewUi;

SideMenu::SideMenu(float width)
{
	UI.BeforeEdit();

	//Main Elem
	AnchorParent* pAnchor{ new AnchorParent() };
	UI.AddChild(pAnchor);

	//Extender
	SizeDef extenderSize;
	extenderSize.HorizontalMode = SizeDef::Pixels;
	extenderSize.VerticalMode = SizeDef::Percentage;
	extenderSize.Value = { width,1.f };
	Extender* pExtender{ new Extender(extenderSize) };
	pAnchor->AddChild(pExtender, { 0,1 });

	//Background
	pExtender->AddChild({ new Box(NewUiSystem::COLOR_MEDIUM) });

	//Margin
	Margin* pMargin{ new Margin(NewUiSystem::BORDER_THICKNESS) };
	pExtender->AddChild({ pMargin });
	m_pExtender = pExtender;

	//Anchor2
	AnchorParent* pAnchorInsideBorder{ new AnchorParent() };
	pMargin->AddChild({ pAnchorInsideBorder });

	//Main List
	ListElem::Settings listSettings;
	listSettings.UniformChildWidth = true;
	ListElem* pList{ new ListElem(listSettings) };
	pAnchorInsideBorder->AddChild(pList, { .5f,1.f });

	//Path
	m_pPath = new SideMenuPath();
	pList->AddChild({ m_pPath });

	//Content Extender
	Extender* pContentExtender{ new Extender(SizeDef{SizeDef::Percentage, SizeDef::Percentage, {1,1}}) };
	pList->AddChild({ pContentExtender });

	//Content Background
	Box* pContentBackground{ new Box(NewUiSystem::COLOR_DARK) };
	pContentExtender->AddChild({ pContentBackground });

	//Content Margin
	Margin* pContentMargin{ new Margin(10) };
	pContentExtender->AddChild({ pContentMargin });

	//Content Anchor
	AnchorParent* pContentAnchor{ new AnchorParent() };
	pContentMargin->AddChild({ pContentAnchor });

	//Content List
	listSettings.ChildMargin = 10.f;
	ListElem* pContentList{ new ListElem(listSettings) };
	pContentAnchor->AddChild({ pContentList, {.5f,1}, {.5f, 1} });

	//Sample content
	pContentList->AddChild({ new Label("First Content Line", NewUiSystem::COLOR_MEDIUM, 13) });
	pContentList->AddChild({ new Label("Second Content Line", NewUiSystem::COLOR_MEDIUM, 13) });

	UI.AfterEdit();
}

void SideMenu::Update()
{
	constexpr float resizeZone{ 15.f };
	const float right{ m_pExtender->GetBounds().GetRight() };
	const Float2 mouse{ MOUSE.GetPos() };

	if (m_Dragging)
	{
		if (!MOUSE.IsLeftBtnDown())
			m_Dragging = false;
	}
	else
	{
		m_Dragging = MOUSE.IsLeftBtnDown() && abs(mouse.x - right) <= resizeZone;
	}

	if (m_Dragging)
	{
		UI.BeforeEdit();
		m_pExtender->SetValueX(mouse.x);
		UI.AfterEdit();
	}

	m_pPath->Update();
}
