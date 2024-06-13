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
			explicit SideMenu(float width = 300.f);

			void Update();

			void Deactivate();
			void SetTab(SideMenuTab& newTab); //Need to be called after a UI.BeforeEdit()

		private:
			Extender* m_pExtender{};
			SideMenuPath* m_pPath{};
			ListElem* m_pContentList{};
			bool m_Dragging{ false };

			SideMenuTab* m_pActiveTab;
		};
	}
}
