#include "pch.h"
#include "SideMenuTab.h"

using namespace NewUi;

SideMenuTab::SideMenuTab(const std::string& title, SideMenuTab* pParent)
	: m_Title{ title }
	, m_pParent{ pParent }
{
	if (m_pParent)
		m_pParent->m_Children.Add(this);
}

SideMenuTab::~SideMenuTab()
{
	m_Children.DeleteAll();
}
