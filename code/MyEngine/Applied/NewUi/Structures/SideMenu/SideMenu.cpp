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
	: m_Width{ width }
{
	UI.BeforeEdit();

	//Main Elem
	AnchorParent* pAnchor{ new AnchorParent() };
	UI.AddChild(pAnchor);

	//Extender
	SizeDef extenderSize;
	extenderSize.HorizontalMode = SizeDef::Pixels;
	extenderSize.VerticalMode = SizeDef::Percentage;
	extenderSize.Value = { 500,1.f };
	Extender* pExtender{ new Extender(extenderSize) };
	pAnchor->AddChild(pExtender, { 0,1 });

	//Background
	pExtender->AddChild({ new Box(NewUiSystem::COLOR_MEDIUM) });

	//Margin
	Margin* pMargin{ new Margin(NewUiSystem::BORDER_THICKNESS) };
	pExtender->AddChild({ pMargin });

	//Anchor2
	AnchorParent* pAnchorInsideBorder{ new AnchorParent() };
	pMargin->AddChild({ pAnchorInsideBorder });

	//Main List
	ListElem::Settings listSettings;
	listSettings.UniformChildWidth = true;
	ListElem* pList{ new ListElem(listSettings) };
	pAnchorInsideBorder->AddChild(pList, { .5f,1.f });

	//Path
	SideMenuPath* pPath{ new SideMenuPath() };
	pList->AddChild({ pPath });

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