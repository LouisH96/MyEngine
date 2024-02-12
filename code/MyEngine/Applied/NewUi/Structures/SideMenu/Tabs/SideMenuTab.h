#pragma once

namespace MyEngine
{
	namespace NewUi
	{
		class ListElem;

		class SideMenuTab
		{
		public:
			explicit SideMenuTab(const std::string& title, SideMenuTab* pParent);
			virtual ~SideMenuTab();

			virtual void Generate(ListElem& parent) = 0;

			const std::string& GetTitle() const { return m_Title; }
			SideMenuTab* GetParent() const { return m_pParent; }

		protected:
			std::string m_Title;

		private:
			SideMenuTab* m_pParent{};
			List<SideMenuTab*> m_Children{};
		};
	}
}
