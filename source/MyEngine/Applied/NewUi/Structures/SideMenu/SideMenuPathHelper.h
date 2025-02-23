#pragma once
#include "SideMenuPath.h"

namespace MyEngine
{
namespace NewUi
{
class SideMenuPath;

class SideMenuPathHelper
{
public:
	explicit SideMenuPathHelper(SideMenuPath& sideMenu);

	const Float2& GetTotalSize() const { return m_TotalSize; }

private:
	SideMenuPath& m_SideMenu;

	Float2 m_TotalSize;

	unsigned m_RowStartIdx;
	unsigned m_CurrentIdx;
	float m_RowHeight;
	Float2 m_CurrentPos;

	void AddButton();
	void AddButtonToCurrentLine(float buttonWidth);
	void FinishLine(unsigned lastIdx);
};
}
}
