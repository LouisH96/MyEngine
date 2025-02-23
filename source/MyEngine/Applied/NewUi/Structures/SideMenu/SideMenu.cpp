#include "pch.h"
#include "SideMenu.h"

#include "SideMenuPath.h"
#include "Tabs/TitleTab.h"
#include <Applied/NewUi/Elements/AnchorParent.h>
#include <Applied/NewUi/Elements/Border.h>
#include <Applied/NewUi/Elements/Box.h>
#include <Applied/NewUi/Elements/Extender.h>
#include <Applied/NewUi/Elements/ListElem.h>
#include <Applied/NewUi/Elements/Margin.h>
#include <Applied/NewUi/NewUiSystem.h>

using namespace NewUi;

SideMenu::SideMenu(float width)
	: m_pActiveTab{ nullptr }
{
	//Extender
	SizeDef extenderSize;
	extenderSize.HorizontalMode = SizeDef::Mode::Pixels;
	extenderSize.VerticalMode = SizeDef::Mode::Percentage;
	extenderSize.Value = { width,1.f };
	Extender* pExtender{ new Extender(extenderSize) };
	UI.AddChild(pExtender);

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
	Extender* pContentExtender{ new Extender(SizeDef{SizeDef::Mode::Percentage, SizeDef::Mode::Percentage, {1,1}}) };
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
	m_pContentList = new ListElem(listSettings);
	pContentAnchor->AddChild({ m_pContentList, {.5f,1}, {.5f, 1} });
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
		m_pExtender->SetSizeDefX(mouse.x);
	}

	//update tab
	SideMenuTab* pNewTab{ m_pPath->GetClickedTab() };
	if (pNewTab && pNewTab != m_pActiveTab)
	{
		m_pPath->SetTab(*pNewTab);
		ActivateNewTab(*pNewTab);
	}
}

void SideMenu::Deactivate()
{
	SideMenuTab* pToDeactivate{ m_pActiveTab };
	while (pToDeactivate)
	{
		pToDeactivate->Deactivate();
		pToDeactivate = pToDeactivate->GetParent();
	}
}

void SideMenu::SetTab(SideMenuTab& newTab)
{
	m_pPath->SetTab(newTab);
	ActivateNewTab(newTab);
}

void SideMenu::RefreshTab()
{
	m_pContentList->DeleteAllChildren();
	m_pActiveTab->Generate(*m_pContentList);
}

void SideMenu::ActivateNewTab(SideMenuTab& newTab)
{
	if (&newTab == m_pActiveTab)
		return;

	const SideMenuTab* const pCommon{ newTab.GetHighestCommonParent(m_pActiveTab) };

	//deactivate current
	if (pCommon != m_pActiveTab)
	{
		SideMenuTab* pToDeactivate{ m_pActiveTab };
		while (pToDeactivate != pCommon)
		{
			pToDeactivate->Deactivate();
			pToDeactivate = pToDeactivate->GetParent();
		}
	}

	//activate new
	if (pCommon != &newTab)
	{
		SideMenuTab* pToActivate{ newTab.GetParentBefore(*pCommon) };
		while (pToActivate != &newTab)
		{
			pToActivate->Activate();
			pToActivate = newTab.GetParentBefore(*pToActivate);
		}
		pToActivate->Activate();
	}

	//ui elements
	m_pContentList->DeleteAllChildren();
	m_pActiveTab = &newTab;
	m_pActiveTab->Generate(*m_pContentList);

	m_pPath->SetTab(*m_pActiveTab);

	UI_TREE.RequestUpdate();
}
