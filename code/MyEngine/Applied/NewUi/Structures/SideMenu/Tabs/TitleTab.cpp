#include "pch.h"
#include "TitleTab.h"

#include "Applied/NewUi/NewUiSystem.h"
#include "Applied/NewUi/Elements/Label.h"
#include "Applied/NewUi/Elements/ListElem.h"

using namespace NewUi;

TitleTab::TitleTab(const std::string& title, SideMenuTab* pParent)
	: SideMenuTab{ title, pParent }
{
}

void TitleTab::Generate(ListElem& parent)
{
	parent.AddChild({ new Label(m_Title, NewUiSystem::COLOR_MEDIUM, 13) });
}
