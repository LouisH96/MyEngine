#include "pch.h"
#include "Keyboard_MsgListener.h"

App::Win32::Keyboard_MsgListener::Keyboard_MsgListener()
{
	for (int i = 0; i < TOTAL_PACKS; i++)
		m_Keys[i] = 0;
}

void App::Win32::Keyboard_MsgListener::PreInput()
{
	for (int i = 0; i < PACKS_PER_STATE; i++)
		m_Keys[i + PACKS_PER_STATE] = m_Keys[i];
}

bool App::Win32::Keyboard_MsgListener::IsDown_(char c) const
{
	const int iPack = c / PACK_SIZE;
	const int iBit = c % PACK_SIZE;
	return m_Keys[iPack] & (1 << iBit);
}

bool App::Win32::Keyboard_MsgListener::IsPressed(char c) const
{
	const int iPack = c / PACK_SIZE;
	const int iBit = c % PACK_SIZE;
	const bool isDown = m_Keys[iPack] & 1 << iBit;
	const bool wasDown = m_Keys[iPack + PACKS_PER_STATE] & 1 << iBit;
	return isDown && !wasDown;
}

bool App::Win32::Keyboard_MsgListener::IsReleased(char c) const
{
	const int iPack = c / PACK_SIZE;
	const int iBit = c % PACK_SIZE;
	const bool isDown = m_Keys[iPack] & 1 << iBit;
	const bool wasDown = m_Keys[iPack + PACKS_PER_STATE] & 1 << iBit;
	return !isDown && wasDown;
}

void App::Win32::Keyboard_MsgListener::KeyUp(char c)
{
	const int iPack = c / PACK_SIZE;
	const int iBit = c % PACK_SIZE;
	m_Keys[iPack] &= ~(1 << iBit);
}

void App::Win32::Keyboard_MsgListener::KeyDown(char c)
{
	const int iPack = c / PACK_SIZE;
	const int iBit = c % PACK_SIZE;
	m_Keys[iPack] |= (1 << iBit);
}
