#include "pch.h"
#include "TitleTab.h"

#include "Ui/UiSystem.h"
#include "Ui/Elements/Label.h"
#include "Ui/Elements/ListElem.h"

using namespace Ui;

TitleTab::TitleTab(const std::string& title, SideMenuTab* pParent)
	: SideMenuTab{ title, pParent }
{
}

void TitleTab::Generate(ListElem& parent)
{
	parent.AddChild({ new Label(m_Title, UiSystem::COLOR_MEDIUM, 13) });
}
