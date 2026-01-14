#include "pch.h"
#include "SideMenu.h"

#include "SideMenuPath.h"
#include "Tabs/TitleTab.h"
#include <Ui/Elements/AnchorParent.h>
#include <Ui/Elements/Border.h>
#include <Ui/Elements/Box.h>
#include <Ui/Elements/Extender.h>
#include <Ui/Elements/ListElem.h>
#include <Ui/Elements/Margin.h>
#include <Ui/UiSystem.h>

using namespace Ui;

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
	pExtender->AddChild({ new Box(UiSystem::COLOR_MEDIUM) });

	//Margin
	Margin* pMargin{ new Margin(UiSystem::BORDER_THICKNESS) };
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
	Box* pContentBackground{ new Box(UiSystem::COLOR_DARK) };
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

	if (m_pRequestedTab)
	{
		pNewTab = m_pRequestedTab;
		m_pRequestedTab = nullptr;
	}

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
	m_pRequestedTab = &newTab;
}

void SideMenu::RefreshTab()
{
	m_pContentList->DeleteAllChildren();
	m_pActiveTab->FullGenerate(*m_pContentList);
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
	m_pActiveTab->FullGenerate(*m_pContentList);

	m_pPath->SetTab(*m_pActiveTab);

	UI_TREE.RequestUpdate();
}
