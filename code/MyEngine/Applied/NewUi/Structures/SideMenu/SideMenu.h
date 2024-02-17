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

			void Update();

			void Deactivate();
			void SetTab(SideMenuTab& newTab);

		private:
			Extender* m_pExtender{};
			SideMenuPath* m_pPath{};
			ListElem* m_pContentList{};
			bool m_Dragging{ false };

			SideMenuTab* m_pActiveTab;
		};
	}
}
