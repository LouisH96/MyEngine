#include "pch.h"
#include "Keyboard_MsgListener.h"

bool MyEngine::App::Window::Keyboard_MsgListener::IsDown_(char c) const
{
	const int iPack = c / PACK_SIZE;
	const int iBit = c % PACK_SIZE;
	return m_Keys[iPack] & (1 << iBit);
}

void MyEngine::App::Window::Keyboard_MsgListener::Reset()
{
	for (unsigned char& key : m_Keys)
		key = 0;
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
