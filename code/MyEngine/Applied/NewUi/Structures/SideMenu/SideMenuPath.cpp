#include "pch.h"
#include "SideMenuPath.h"

#include "SideMenuPathHelper.h"
#include "Applied/NewUi/NewUiSystem.h"
#include "Gui/GuiRenderer.h"

using namespace NewUi;

const Float2 SideMenuPath::BUTTON_MARGIN{ 7,4 };

SideMenuPath::SideMenuPath()
	: m_ArrowSize{ NEW_FONT.GetTextSize_XCenter(">", FONT_SIZE) }
	, m_PointsSize{ NEW_FONT.GetTextSize_XCenter("...", FONT_SIZE) }
{
	AddChild("Main Menu");
	AddChild("Pawns");
	AddChild("Rook");
	AddChild("Collision Mesh");
	AddChild("Something else");
}

void SideMenuPath::Update()
{
	if (!MOUSE.IsLeftBtnPressed())
		return;

	const Float2 mouse{ MOUSE.GetPosLeftBot() - GetPosition()};

	for (unsigned i = 0; i < m_ButtonInfo.GetSize(); ++i)
	{
		ButtonInfo& button{ m_ButtonInfo[i] };
		if (RectFloat::ContainsPoint(button.Pos, button.Size, mouse))
		{
			Logger::Print("Clicked", button.Text);
			break;
		}
	}
}

void SideMenuPath::AddChild(const std::string& title)
{
	const Float2 textSize{ NEW_FONT.GetTextSize_XCenter(title, FONT_SIZE) };

	ButtonInfo button{};
	button.Text = title;
	button.Size = textSize + BUTTON_MARGIN * 2;

	m_ButtonInfo.Add(button);
}

void SideMenuPath::UpdateSizeAndTreePositions(const ResizePref& pref)
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
		GUI.Remove(info.BackgroundId);
		NEW_FONT.Remove(info.TextId);

		if (info.ArrowId != Uint::MAX)
		{
			NEW_FONT.Remove(info.ArrowId);
			info.ArrowId = Uint::MAX;
		}
	}
}

void SideMenuPath::Create()
{
	for (unsigned i = 0; i < m_ButtonInfo.GetSize(); ++i)
	{
		ButtonInfo& info{ m_ButtonInfo[i] };

		const std::string& buttonText{ info.DisplayPoints ? "..." : info.Text };

		const Float2 textSize{ NEW_FONT.GetTextSize_XCenter(buttonText, FONT_SIZE) };
		const Float2 buttonSize{ info.DisplayPoints
			? Float2{ m_PointsSize.x + BUTTON_MARGIN.x * 2, info.Size.y}
			: info.Size };

		const Float2 textPosition{ GetPosition() + info.Pos + (buttonSize - textSize) / 2 };
		const Float2 buttonPos{ GetPosition() + info.Pos };

		info.BackgroundId = GUI.Add({ -1,-1 }, buttonPos, buttonSize, NewUiSystem::COLOR_DARK);
		info.TextId = NEW_FONT.Add_XCenter({ buttonText, FONT_SIZE, NewUiSystem::COLOR_MEDIUM }, textPosition);

		if (i == 0)
			continue;

		const Float2 arrowPos{ buttonPos.x - m_ArrowSize.x - ARROW_MARGIN, buttonPos.y + (buttonSize.y - m_ArrowSize.y) / 2 };
		info.ArrowId = NEW_FONT.Add_XCenter({ ">", FONT_SIZE, NewUiSystem::COLOR_DARK }, arrowPos);
	}
}
