#include "pch.h"
#include "SideMenuTab.h"

#include "Applied/NewUi/Structures/SideMenu/SideMenu.h"

using namespace NewUi;

SideMenuTab::SideMenuTab(const std::string& title, SideMenuTab* pParent)
	: m_Title{ title }
	, m_pParent{ pParent }
{
}
