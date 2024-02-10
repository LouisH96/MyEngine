#include "pch.h"
#include "SideMenuPath.h"

#include "Applied/NewUi/NewUiSystem.h"
#include "Gui/GuiRenderer.h"

using namespace NewUi;

const Float2 SideMenuPath::MARGIN{ 7,4 };

SideMenuPath::SideMenuPath()
	: m_ArrowWidth{ NEW_FONT.GetTextSize_XCenter(">", FONT_SIZE).x }
	, m_PointsWidth{ NEW_FONT.GetTextSize_XCenter("...", FONT_SIZE).x }
{
	AddChild("Main Menu");
	AddChild("Pawns");
	AddChild("Rook");
	AddChild("Collision Mesh");
	AddChild("Something else");
}

void SideMenuPath::AddChild(const std::string& title)
{
	const Float2 textSize{ NEW_FONT.GetTextSize_XCenter(title, FONT_SIZE) };
	const Float2 buttonSize{ textSize + MARGIN * 2 };

	m_ButtonInfo.Add(ButtonInfo{ title, buttonSize.x });
	m_MaxButtonHeight = Float::Max(m_MaxButtonHeight, buttonSize.y);

	const Float2 arrowSize{ NEW_FONT.GetTextSize_XCenter(">", FONT_SIZE) };
	m_ArrowVerOffset = (m_MaxButtonHeight - arrowSize.y) / 2;

	const Float2 pointsSize{ NEW_FONT.GetTextSize_XCenter("...", FONT_SIZE) };
	m_PointsVerOffset = (m_MaxButtonHeight - pointsSize.y) / 2.f;
}

void SideMenuPath::UpdateSizeAndTreePositions(const ResizePref& pref)
{
	constexpr float minHeight{ 5.f };
	const float height{ Float::Max(minHeight, m_MaxButtonHeight) };

	SetSize({ pref.maxSize.x, height });
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
	Float2 position{ GetPosition() };
	Float2 buttonSize{ 0, GetHeight() };

	position.x += GetWidth();

	for (unsigned i = m_ButtonInfo.GetSize() - 1; i + 1 != 0; i--)
	{
		ButtonInfo& info{ m_ButtonInfo[i] };
		buttonSize.x = info.Width;

		position.x -= buttonSize.x;

		const Float2 textSize{ NEW_FONT.GetTextSize_XCenter(info.Text, FONT_SIZE) };
		const Float2 textPosition{ position + (buttonSize - textSize) / 2 };

		info.BackgroundId = GUI.Add({ -1,-1 }, position, buttonSize, NewUiSystem::COLOR_DARK);
		info.TextId = NEW_FONT.Add_XCenter({ info.Text, FONT_SIZE, NewUiSystem::COLOR_MEDIUM }, textPosition);

		position.x -= NewUiSystem::BORDER_THICKNESS + m_ArrowWidth;
		if (i != 0)
			info.ArrowId = NEW_FONT.Add_XCenter({ ">", FONT_SIZE, NewUiSystem::COLOR_DARK }, { position.x, position.y + m_ArrowVerOffset });
		position.x -= NewUiSystem::BORDER_THICKNESS;
	}
}
