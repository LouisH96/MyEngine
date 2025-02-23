#pragma once
#include <Applied\NewUi\Structures\SideMenu\SideMenu.h>

namespace MyEngine
{
namespace NewUi
{
class TabManager
{
public:
	TabManager(float width = 300);
	TabManager(SideMenuTab& initTab, float width = 300);
	virtual ~TabManager() = default;

	void Update();
	void SetTab(SideMenuTab& newTab);
	void RefreshTab();

private:
	SideMenu m_SideMenu;
};
}
}
