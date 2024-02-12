#pragma once

namespace MyEngine
{
	namespace NewUi
	{
		class SideMenuTab;
		class ListElem;
		class SideMenuPath;
		class Extender;

		class SideMenu
		{
		public:
			explicit SideMenu(float width);
			~SideMenu();

			void Update();
			void SetRootTab(SideMenuTab* pTab);

			//temp
			void ActivateTab(SideMenuTab* pTab);

		private:
			Extender* m_pExtender{};
			SideMenuPath* m_pPath{};
			ListElem* m_pContentList{};
			bool m_Dragging{ false };

			SideMenuTab* m_pRootTab;
			SideMenuTab* m_pActiveTab;
		};
	}
}
