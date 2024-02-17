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

			virtual void Activate() {}
			virtual void Deactivate() {}

			void SetTitle(const std::string& title) { m_Title = title; }
			const std::string& GetTitle() const { return m_Title; }
			SideMenuTab* GetParent() const { return m_pParent; }

			SideMenuTab* GetRootParent();
			const SideMenuTab* GetRootParent() const;

			SideMenuTab* GetParentBefore(const SideMenuTab& higherParent);
			const SideMenuTab* GetParentBefore(const SideMenuTab& higherParent) const;

			SideMenuTab* GetHighestCommonParent(const SideMenuTab* pOther);

		protected:
			std::string m_Title;

		private:
			SideMenuTab* m_pParent;
		};
	}
}
