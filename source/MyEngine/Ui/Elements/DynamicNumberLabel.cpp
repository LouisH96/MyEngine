#include "pch.h"
#include "DynamicNumberLabel.h"

using namespace Ui;

DynamicNumberLabel::DynamicNumberLabel(unsigned maxLength, const Float3& color, float fontSize)
	: DynamicLabel{
		std::string(maxLength, '0'),
		UI_FONT.GetTextAssembler().GetMaxNumberBounds(fontSize).GetMaxTextBounds(maxLength),
		color,
		fontSize
	}
	, m_MaxUnits{ maxLength }
{
}

void DynamicNumberLabel::SetNumber(unsigned number)
{
	ToString(number, m_Text);
	UpdateText();
}

void DynamicNumberLabel::ToString(unsigned number, std::string& str)
{
	str.resize(m_MaxUnits);
	for (unsigned iNumber{ 0 }; iNumber < m_MaxUnits; ++iNumber)
	{
		if (number == 0 && iNumber != 0)
		{
			const unsigned shift{ m_MaxUnits - iNumber };

			memcpy(&str[0], &str[shift], iNumber);
			str.resize(iNumber);
			break;
		}

		const unsigned unitValue{ number % 10 };
		number /= 10;

		const char character{ static_cast<char>('0' + unitValue) };
		str[m_MaxUnits - 1 - iNumber] = character;
	}
}
