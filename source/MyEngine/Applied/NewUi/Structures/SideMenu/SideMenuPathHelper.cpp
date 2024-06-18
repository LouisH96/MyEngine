#include "pch.h"
#include "SideMenuPathHelper.h"

#include "Applied/NewUi/NewUiSystem.h"

using namespace NewUi;

SideMenuPathHelper::SideMenuPathHelper(SideMenuPath& sideMenu)
	: m_SideMenu{ sideMenu }
	, m_TotalSize{ sideMenu.GetWidth(), 0 }
	, m_RowStartIdx{ 0 }
	, m_CurrentIdx{ 0 }
	, m_RowHeight{ 0 }
	, m_CurrentPos{ 0 }
{
	while (m_CurrentIdx < m_SideMenu.m_ButtonInfo.GetSize())
		AddButton();

	if (m_RowStartIdx != m_CurrentIdx)
		FinishLine(m_CurrentIdx - 1);

	for (unsigned i = 0; i < m_SideMenu.m_ButtonInfo.GetSize(); ++i)
	{
		SideMenuPath::ButtonInfo& button{ m_SideMenu.m_ButtonInfo[i] };
		button.Pos.y += m_SideMenu.GetHeight();
	}
	m_TotalSize.y = -m_CurrentPos.y - SideMenuPath::ROW_MARGIN;
}

void SideMenuPathHelper::AddButton()
{
	SideMenuPath::ButtonInfo& button{ m_SideMenu.m_ButtonInfo[m_CurrentIdx] };

	if (m_CurrentPos.x + button.Size.x <= m_TotalSize.x)
	{
		//fits
		AddButtonToCurrentLine(button.Size.x);
		button.DisplayPoints = false;
	}
	else
	{
		//no fit

		//does it fit on a new line?
		const float newLineXPos{ m_SideMenu.m_ArrowSize.x + SideMenuPath::ARROW_MARGIN * 2 };
		if (newLineXPos + button.Size.x <= m_TotalSize.x)
		{
			//add on new line
			FinishLine(m_CurrentIdx - 1);

			m_CurrentPos.x = newLineXPos;
			AddButtonToCurrentLine(button.Size.x);
			button.DisplayPoints = false;
		}
		else
		{
			const float pointsButtonWidth{ m_SideMenu.m_PointsSize.x + SideMenuPath::BUTTON_MARGIN.x * 2 };
			//try fit ... on current line
			if (m_CurrentPos.x + pointsButtonWidth > m_TotalSize.x && m_CurrentIdx != m_RowStartIdx)
			{
				//else add ... on new line
				FinishLine(m_CurrentIdx - 1);
				m_CurrentPos.x = newLineXPos;
			}

			AddButtonToCurrentLine(pointsButtonWidth);
			button.DisplayPoints = true;
		}
	}

	++m_CurrentIdx;
}

void SideMenuPathHelper::AddButtonToCurrentLine(float buttonWidth)
{
	SideMenuPath::ButtonInfo& button{ m_SideMenu.m_ButtonInfo[m_CurrentIdx] };

	button.Pos.x = m_CurrentPos.x;
	m_CurrentPos.x += buttonWidth + SideMenuPath::ARROW_MARGIN * 2 + m_SideMenu.m_ArrowSize.x;

	m_RowHeight = Float::Max(m_RowHeight, button.Size.y);
}

void SideMenuPathHelper::FinishLine(unsigned lastIdx)
{
	m_CurrentPos.y -= m_RowHeight;
	for (unsigned i = m_RowStartIdx; i <= lastIdx; i++)
	{
		SideMenuPath::ButtonInfo& button{ m_SideMenu.m_ButtonInfo[i] };
		button.Size.y = m_RowHeight;
		button.Pos.y = m_CurrentPos.y;
	}

	m_CurrentPos.y -= SideMenuPath::ROW_MARGIN;
	m_RowStartIdx = lastIdx + 1;
	m_RowHeight = 0;
}
