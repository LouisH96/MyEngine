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

	m_pLabel = new DynamicNumberLabel(NR_CHAR, Color::Yellow, FONT_SCALE);
	m_pLabel->SetPivot({ 1,0 });
	pMargin->AddChild(m_pLabel);

	m_pLabel->SetNumber(0);
}

void Rendering::FpsDisplay::SetFps(unsigned fps)
{
	m_pLabel->SetNumber(fps);
}

