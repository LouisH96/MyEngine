#include "pch.h"
#include "SideMenuPath.h"

#include "SideMenuPathHelper.h"
#include "Tabs/SideMenuTab.h"
#include <Ui/UiSystem.h>

using namespace Ui;

const Float2 SideMenuPath::BUTTON_MARGIN{ 7,4 };

SideMenuPath::SideMenuPath()
	: m_ArrowSize{ UI_FONT.GetTextSize_XCenter(">", FONT_SIZE) }
	, m_PointsSize{ UI_FONT.GetTextSize_XCenter("...", FONT_SIZE) }
{
}

SideMenuTab* SideMenuPath::GetClickedTab() const
{
	if (!MOUSE.IsLeftBtnPressed())
		return nullptr;

	const Float2 mouse{ MOUSE.GetPosLeftBot() - GetPosition() };

	for (unsigned i = 0; i < m_ButtonInfo.GetSize(); ++i)
	{
		const ButtonInfo& button{ m_ButtonInfo[i] };
		if (RectFloat::ContainsPoint(button.Pos, button.Size, mouse))
		{
			return button.pTab;
		}
	}

	return nullptr;
}

void SideMenuPath::SetTab(SideMenuTab& tab)
{
	for (unsigned iButton{ 0 }; iButton < m_ButtonInfo.GetSize(); ++iButton)
		m_ButtonInfo[iButton].ClearGraphics();
	m_ButtonInfo.Clear();

	AddSelfAfterParent(tab);
	RequestUpdate();
}

void SideMenuPath::AddButton(SideMenuTab& tab)
{
	const Float2 textSize{ UI_FONT.GetTextSize_XCenter(tab.GetTitle(), FONT_SIZE) };

	ButtonInfo button{};
	button.Text = tab.GetTitle();
	button.Size = textSize + BUTTON_MARGIN * 2;
	button.pTab = &tab;

	m_ButtonInfo.Add(button);
}

void SideMenuPath::TreeUpdate(const ResizePref& pref)
{
	SetWidth(pref.maxSize.x);

	const SideMenuPathHelper helper{ *this };

	constexpr float minHeight{ 5.f };
	const float height{ Float::Max(minHeight, helper.GetTotalSize().y) };

	SetHeight(height);
}

void SideMenuPath::Clear()
{
	for (unsigned i = 0; i < m_ButtonInfo.GetSize(); i++)
	{
		ButtonInfo& info{ m_ButtonInfo[i] };
		info.ClearGraphics();
	}
}

void SideMenuPath::Create()
{
	for (unsigned i = 0; i < m_ButtonInfo.GetSize(); ++i)
	{
		ButtonInfo& info{ m_ButtonInfo[i] };

		const std::string& buttonText{ info.DisplayPoints ? "..." : info.Text };

		const Float2 textSize{ UI_FONT.GetTextSize_XCenter(buttonText, FONT_SIZE) };
		const Float2 buttonSize{ info.DisplayPoints
			? Float2{ m_PointsSize.x + BUTTON_MARGIN.x * 2, info.Size.y}
			: info.Size };

		const Float2 textPosition{ GetPosition() + info.Pos + (buttonSize - textSize) / 2 };
		const Float2 buttonPos{ GetPosition() + info.Pos };

		UiFontRenderer::TextInfo text{};
		text.Text = buttonText;
		text.Scale = FONT_SIZE;
		text.Color = UiSystem::COLOR_MEDIUM;
		text.Position = textPosition;

		info.BackgroundId = UI_RECT.Add({ buttonPos, buttonSize }, UiSystem::COLOR_DARK);
		info.TextId = UI_FONT.Add(text);

		if (i == 0)
			continue;

		const Float2 arrowPos{ buttonPos.x - m_ArrowSize.x - ARROW_MARGIN, buttonPos.y + (buttonSize.y - m_ArrowSize.y) / 2 };
		text.Text = ">";
		text.Color = UiSystem::COLOR_DARK;
		text.Position = arrowPos;
		info.ArrowId = UI_FONT.Add(text);
	}
}

void SideMenuPath::AddSelfAfterParent(SideMenuTab& tab)
{
	if (tab.GetParent())
		AddSelfAfterParent(*tab.GetParent());
	AddButton(tab);
}

void SideMenuPath::ButtonInfo::ClearGraphics()
{
	BackgroundId.Clear();
	TextId.Clear();
	ArrowId.Clear();
}
