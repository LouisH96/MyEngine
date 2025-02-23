#include "pch.h"
#include "TabManager.h"

using namespace MyEngine;
using namespace Ui;

TabManager::TabManager(float width)
	: m_SideMenu{ width }
{
}

TabManager::TabManager(SideMenuTab& initTab, float width)
	: m_SideMenu{ width }
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

void TabManager::RefreshTab()
{
	m_SideMenu.RefreshTab();
}