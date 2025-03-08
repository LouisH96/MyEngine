#include "pch.h"
#include "FpsDisplay.h"

#include <Io/Ttf/TtfReader.h>
#include <Ui\Elements\Extender.h>
#include <Ui\Elements\Margin.h>
#include <Ui\Elements\Box.h>

using namespace MyEngine;

Rendering::FpsDisplay::FpsDisplay()
{
	const TextAssembler& assembler{ UI_FONT.GetTextAssembler() };

	const TextAssembler::MaxCharBounds maxNumber{ assembler.GetMaxNumberBounds(FONT_SCALE) };
	const Float2 maxLabelSize{ maxNumber.GetMaxTextBounds(NR_CHAR) };

	AnchorChild rootChild{};
	rootChild.SetFillMin();
	rootChild.SetAnchors({ 1,0 });

	Margin* pMargin{ new Margin(5.f) };
	rootChild.pChild = pMargin;
	UI_ROOT.AddChild(rootChild);

	Extender* pExtender{ new Extender(SizeDef::Pixels(maxLabelSize)) };
	pMargin->AddChild(pExtender);

	m_pLabel = new DynamicLabel(std::string(NR_CHAR, '0'), Color::Yellow, FONT_SCALE);
	m_pLabel->SetPivot({ 1,0 });
	pExtender->AddChild(m_pLabel);

	SetText(m_Fps);
}

void Rendering::FpsDisplay::SetFps(unsigned fps)
{
	m_Fps = fps;
	SetText(fps);
}

void Rendering::FpsDisplay::SetText(unsigned fps)
{
	std::string& text{ m_pLabel->GetText() };

	for (unsigned iChar{ 0 }; iChar < NR_CHAR; ++iChar)
	{
		char character;
		if (fps == 0)
			character = ' ';
		else
		{
			const unsigned number{ fps % 10 };
			fps /= 10;
			character = static_cast<char>('0' + number);
		}
		text[NR_CHAR - 1 - iChar] = character;
	}
	m_pLabel->UpdateText();
}
