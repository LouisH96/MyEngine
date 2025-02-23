#include "pch.h"
#include "SideMenuTab.h"

using namespace Ui;

SideMenuTab::SideMenuTab(const std::string& title, SideMenuTab* pParent)
	: m_Title{ title }
	, m_pParent{ pParent }
{
}

SideMenuTab* SideMenuTab::GetRootParent()
{
	SideMenuTab* pRoot{ this };

	while (pRoot->GetParent())
		pRoot = pRoot->GetParent();

	return pRoot;
}

const SideMenuTab* SideMenuTab::GetRootParent() const
{
	const SideMenuTab* pRoot{ this };

	while (pRoot->GetParent())
		pRoot = pRoot->GetParent();

	return pRoot;
}

SideMenuTab* SideMenuTab::GetParentBefore(const SideMenuTab& higherParent)
{
	SideMenuTab* pParent{ this };

	while (pParent->GetParent() != &higherParent)
		pParent = pParent->GetParent();

	return pParent;
}

const SideMenuTab* SideMenuTab::GetParentBefore(const SideMenuTab& higherParent) const
{
	const SideMenuTab* pParent{ this };

	while (pParent->GetParent() != &higherParent)
		pParent = pParent->GetParent();

	return pParent;
}

SideMenuTab* SideMenuTab::GetHighestCommonParent(const SideMenuTab* pOther)
{
	if (!pOther)
		return nullptr;

	SideMenuTab* pCommon{ nullptr };

	const SideMenuTab* pOtherParent{ pOther->GetRootParent() };
	SideMenuTab* pCurrentParent{ GetRootParent() };

	while (pOtherParent == pCurrentParent)
	{
		pCommon = pCurrentParent;

		if (pCommon == this || pCommon == pOther)
			return pCommon;

		pOtherParent = pOther->GetParentBefore(*pCommon);
		pCurrentParent = GetParentBefore(*pCommon);
	}
	return pCommon;
}
