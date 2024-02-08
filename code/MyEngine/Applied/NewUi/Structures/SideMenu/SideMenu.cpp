#include "pch.h"
#include "SideMenu.h"

#include "Applied/NewUi/NewUiSystem.h"
#include "Applied/NewUi/Elements/AnchorParent.h"
#include "Applied/NewUi/Elements/Border.h"
#include "Applied/NewUi/Elements/Box.h"
#include "Applied/NewUi/Elements/Extender.h"
#include "Applied/NewUi/Elements/Label.h"
#include "Applied/NewUi/Elements/ListElem.h"
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

	//Border
	Border* pBorder{ new Border() };
	pExtender->AddChild({ pBorder });

	//Anchor2
	AnchorParent* pAnchorInsideBorder{ new AnchorParent() };
	pBorder->AddChild({ pAnchorInsideBorder });

	//Main List
	ListElem::Settings listSettings;
	listSettings.UniformChildWidth = true;
	ListElem* pList{ new ListElem(listSettings) };
	pAnchorInsideBorder->AddChild(pList, { .5f,1.f });

	//Title
	Label* pTitle{ new Label("SideMenu") };
	pList->AddChild({ pTitle });

	UI.AfterEdit();
}