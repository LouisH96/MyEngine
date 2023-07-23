#include "pch.h"
#include "FpsDisplay.h"

#include <App/Win32/Window.h>
#include <Gui/FontRenderer.h>
#include <Io/Ttf/TtfReader.h>

Rendering::FpsDisplay::FpsDisplay()
{
	SetText();
}
void Rendering::FpsDisplay::Render()
{
	Globals::pFontRenderer->Remove(m_TextId);
	SetText();
}

void Rendering::FpsDisplay::SetText()
{
	constexpr unsigned nrNumbers{ 4 };
	char chars[nrNumbers + 1];
	chars[nrNumbers - 1] = '0';
	chars[nrNumbers] = '\0';
	unsigned first{ 3 };

	float fps{ static_cast<float>(m_Fps) };
	for (int i = 0; i < nrNumbers; i++)
	{
		const int current{ static_cast<int>(fps / 1000) };
		if (current > 0 && i < first)
			first = i;
		chars[i] = '0' + current;
		fps -= static_cast<float>(current * 1000);
		fps *= 10;
	}
	m_TextId = Globals::pFontRenderer->AddRightBot({ &chars[first] }, 9, { -5, 5 }, { 1,1,0 }, 1);
}
