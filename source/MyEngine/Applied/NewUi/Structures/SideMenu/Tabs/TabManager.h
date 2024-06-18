#pragma once
#include <Applied\NewUi\Structures\SideMenu\SideMenu.h>

namespace MyEngine
{
	namespace NewUi
	{
		class TabManager
		{
		public:
			TabManager() = default;
			TabManager(SideMenuTab& initTab);
			virtual ~TabManager() = default;

			void Update();
			void SetTab(SideMenuTab& newTab);

		private:
			SideMenu m_SideMenu;
		};
	}
}
