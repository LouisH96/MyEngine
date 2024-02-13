#pragma once

namespace MyEngine
{
	namespace NewUi
	{
		class SideMenu;
		class ListElem;

		class SideMenuTab
		{
		public:
			explicit SideMenuTab(const std::string& title, SideMenuTab* pParent);
			virtual ~SideMenuTab() = default;

			virtual void Generate(ListElem& parent) = 0;

			const std::string& GetTitle() const { return m_Title; }
			SideMenuTab* GetParent() const { return m_pParent; }

		protected:
			std::string m_Title;

		private:
			SideMenuTab* m_pParent;
		};
	}
}
