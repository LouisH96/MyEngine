#include "pch.h"
#include "Mouse.h"
#include <Windows.h>
#include <windowsx.h>

bool App::Win32::Mouse::IsLeftBtnDown() const
{
	return m_State & LEFT_MASK;
}

bool App::Win32::Mouse::IsLeftBtnPressed() const
{
	return (m_State & (LEFT_MASK | (LEFT_MASK << NR_BUTTONS))) == LEFT_MASK;
}

bool App::Win32::Mouse::IsLeftBtnReleased() const
{
	return (m_State & (LEFT_MASK | LEFT_MASK << NR_BUTTONS)) == (LEFT_MASK << NR_BUTTONS);
}

bool App::Win32::Mouse::IsMiddleBtnDown() const
{
	return m_State & MIDDLE_MASK;
}

bool App::Win32::Mouse::IsMiddleBtnPressed() const
{
	return (m_State & (MIDDLE_MASK | (MIDDLE_MASK << NR_BUTTONS))) == MIDDLE_MASK;
}

bool App::Win32::Mouse::IsMiddleBtnReleased() const
{
	return (m_State & (MIDDLE_MASK | MIDDLE_MASK << NR_BUTTONS)) == (MIDDLE_MASK << NR_BUTTONS);
}

bool App::Win32::Mouse::IsRightBtnDown() const
{
	return m_State & RIGHT_MASK;
}

bool App::Win32::Mouse::IsRightBtnPressed() const
{
	return (m_State & (RIGHT_MASK | (RIGHT_MASK << NR_BUTTONS))) == RIGHT_MASK;
}

bool App::Win32::Mouse::IsRightBtnReleased() const
{
	return (m_State & (RIGHT_MASK | RIGHT_MASK << NR_BUTTONS)) == (RIGHT_MASK << NR_BUTTONS);
}

void App::Win32::Mouse::SetPos(Int2 position)
{
	m_Pos = position;
}

void App::Win32::Mouse::PreChange()
{
	m_Movement = m_Pos; //temp store old position in m_Movement
	m_Scroll = 0;
	m_State &= LEFT_MASK | MIDDLE_MASK | RIGHT_MASK;
	m_State |= m_State << NR_BUTTONS;
}

void App::Win32::Mouse::PostChange()
{
	m_Movement = m_Pos - m_Movement;
}

void App::Win32::Mouse::OnMove(long long position)
{
	m_Pos = { GET_X_LPARAM(position), GET_Y_LPARAM(position) };
}

void App::Win32::Mouse::OnScroll(float scroll)
{
	m_Scroll = scroll;
}

void App::Win32::Mouse::OnLeftBtnPressed()
{
	m_State |= LEFT_MASK;
}

void App::Win32::Mouse::OnLeftBtnReleased()
{
	m_State &= ~LEFT_MASK;
}

void App::Win32::Mouse::OnMiddleBtnPressed()
{
	m_State |= MIDDLE_MASK;
}

void App::Win32::Mouse::OnMiddleBtnReleased()
{
	m_State &= ~MIDDLE_MASK;
}

void App::Win32::Mouse::OnRightBtnPressed()
{
	m_State |= RIGHT_MASK;
}

void App::Win32::Mouse::OnRightBtnReleased()
{
	m_State &= ~RIGHT_MASK;
}
