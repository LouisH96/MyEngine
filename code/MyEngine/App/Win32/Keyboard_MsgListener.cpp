#include "pch.h"
#include "Keyboard_MsgListener.h"

MyEngine::App::Window::Keyboard_MsgListener::Keyboard_MsgListener()
{
	for (int i = 0; i < NR_PACKS; i++)
		m_Keys[i] = 0;
}

bool MyEngine::App::Window::Keyboard_MsgListener::IsDown_(char c) const
{
	const int iPack = c / PACK_SIZE;
	const int iBit = c % PACK_SIZE;
	return m_Keys[iPack] & (1 << iBit);
}

void MyEngine::App::Window::Keyboard_MsgListener::KeyUp(char c)
{
	const int iPack = c / PACK_SIZE;
	const int iBit = c % PACK_SIZE;
	m_Keys[iPack] &= ~(1 << iBit);
}

void MyEngine::App::Window::Keyboard_MsgListener::KeyDown(char c)
{
	const int iPack = c / PACK_SIZE;
	const int iBit = c % PACK_SIZE;
	m_Keys[iPack] |= (1 << iBit);
}
