#include "pch.h"
#include "Mouse.h"
#include <Windows.h>
#include <windowsx.h>

#include "Window.h"
#include "Rendering/Canvas.h"

using namespace App::Win32;

bool Mouse::IsLeftBtnDown() const
{
	return m_State & LEFT_MASK;
}

bool Mouse::IsLeftBtnPressed() const
{
	return (m_State & (LEFT_MASK | (LEFT_MASK << NR_BUTTONS))) == LEFT_MASK;
}

bool Mouse::IsLeftBtnReleased() const
{
	return (m_State & (LEFT_MASK | LEFT_MASK << NR_BUTTONS)) == (LEFT_MASK << NR_BUTTONS);
}

bool Mouse::IsMiddleBtnDown() const
{
	return m_State & MIDDLE_MASK;
}

bool Mouse::IsMiddleBtnPressed() const
{
	return (m_State & (MIDDLE_MASK | (MIDDLE_MASK << NR_BUTTONS))) == MIDDLE_MASK;
}

bool Mouse::IsMiddleBtnReleased() const
{
	return (m_State & (MIDDLE_MASK | MIDDLE_MASK << NR_BUTTONS)) == (MIDDLE_MASK << NR_BUTTONS);
}

bool Mouse::IsRightBtnDown() const
{
	return m_State & RIGHT_MASK;
}

bool Mouse::IsRightBtnPressed() const
{
	return (m_State & (RIGHT_MASK | (RIGHT_MASK << NR_BUTTONS))) == RIGHT_MASK;
}

bool Mouse::IsRightBtnReleased() const
{
	return (m_State & (RIGHT_MASK | RIGHT_MASK << NR_BUTTONS)) == (RIGHT_MASK << NR_BUTTONS);
}

void Mouse::SetPos(Int2 position)
{
	m_Pos = position;
}

Float2 Mouse::GetPosNdc() const
{
	return{
		(static_cast<float>(m_Pos.x) / CANVAS.GetWidthF() - .5f) * 2.f,
		(static_cast<float>(m_Pos.y) / CANVAS.GetHeightF() - .5f) * -2.f,
	};
}

Float2 Mouse::GetPosLeftBot() const
{
	return { static_cast<float>(m_Pos.x), CANVAS.GetHeightF() - m_Pos.y };
}

void Mouse::SetPosRuntime(Int2 position)
{
	tagPOINT pos{ position.x,position.y};
	ClientToScreen(WINDOW.GetWindowHandle(), &pos);
	SetCursorPos(pos.x, pos.y);
	m_Pos = position;
}

void Mouse::PreChange()
{
	m_Movement = m_Pos; //temp store old position in m_Movement
	m_Scroll = 0;
	m_State &= LEFT_MASK | MIDDLE_MASK | RIGHT_MASK;
	m_State |= m_State << NR_BUTTONS;
}

void Mouse::PostChange()
{
	m_Movement = m_Pos - m_Movement;
}

void Mouse::OnMove(long long position)
{
	m_Pos = { GET_X_LPARAM(position), GET_Y_LPARAM(position) };
}

void Mouse::OnScroll(float scroll)
{
	m_Scroll = scroll;
}

void Mouse::OnLeftBtnPressed()
{
	m_State |= LEFT_MASK;
}

void Mouse::OnLeftBtnReleased()
{
	m_State &= ~LEFT_MASK;
}

void Mouse::OnMiddleBtnPressed()
{
	m_State |= MIDDLE_MASK;
}

void Mouse::OnMiddleBtnReleased()
{
	m_State &= ~MIDDLE_MASK;
}

void Mouse::OnRightBtnPressed()
{
	m_State |= RIGHT_MASK;
}

void Mouse::OnRightBtnReleased()
{
	m_State &= ~RIGHT_MASK;
}
