#include "pch.h"
#include "DynamicNumberLabel.h"

using namespace Ui;

DynamicNumberLabel::DynamicNumberLabel(unsigned maxLength, const Float3& color, float fontSize)
	: DynamicLabel{
		std::string(maxLength, '0'),
		UI_FONT.GetTextAssembler().GetMaxNumberBounds(fontSize).GetMaxTextBounds(maxLength),
		color,
		fontSize }
{
	SetPivot({ 1,0 });
}

void DynamicNumberLabel::SetNumber(unsigned number)
{
	ToString(number, m_Text);
	UpdateText();
}

void DynamicNumberLabel::ToString(unsigned number, std::string& str)
{
	/*
		todo: the spaces should be removed.The string should just be shorter.
			Did it like this because the plan was to only change the existing
			vertices. But that's not necessary in the current implementation.
	*/
	for (unsigned iChar{ 0 }; iChar < str.size(); ++iChar)
	{
		char character;
		if (number == 0 && iChar != 0)
			character = ' ';
		else
		{
			const unsigned unitValue{ number % 10 };
			number /= 10;
			character = static_cast<char>('0' + unitValue);
		}
		str[str.size() - 1 - iChar] = character;
	}
}
