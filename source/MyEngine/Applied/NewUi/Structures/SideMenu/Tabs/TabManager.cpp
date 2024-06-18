#include "pch.h"
#include "TabManager.h"

using namespace MyEngine;
using namespace NewUi;

TabManager::TabManager(SideMenuTab& initTab)
{
	SetTab(initTab);
}

void TabManager::Update()
{
	m_SideMenu.Update();
}

void TabManager::SetTab(SideMenuTab& newTab)
{
	m_SideMenu.SetTab(newTab);
}
