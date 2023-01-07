#include "pch.h"
#include <Windows.h>
#include "Keyboard.h"
#include <iostream>

void MyEngine::App::Input::Keyboard::Update()
{
	unsigned char keys[NR_KEYS];
	GetKeyboardState(keys);

	Pack(keys, m_Keys);
}

bool MyEngine::App::Input::Keyboard::IsDown(unsigned char c) const
{
	const int iPack = c / PACK_SIZE;
	const int iBit = c % PACK_SIZE;
	return m_Keys[iPack] & (1 << iBit);
}

void MyEngine::App::Input::Keyboard::Pack(const unsigned char* pSource, unsigned char* pDest)
{
	//source is array of char's per keyboard key
	//the highest bit in the source bits is whether the key is up/down (0/1)
	//dest will be array of chars where each bit represents a key up/down (0/1)
	for (ptrdiff_t iPack = 0; iPack < NR_PACKS; iPack++)
	{
		const unsigned char* pUnpacked = &pSource[iPack * 8];
		unsigned char& packed = pDest[iPack];
		packed = 0;
		for (int8_t iChar = 0; iChar < 8; iChar++)
		{
			const int8_t mask = static_cast<int8_t>(*pUnpacked++ & (1 << 7));
			packed |= (mask >> (7 - iChar));
		}
	}
}
