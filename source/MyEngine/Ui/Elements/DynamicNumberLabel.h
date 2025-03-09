#pragma once

#include "DynamicLabel.h"

namespace MyEngine
{
namespace Ui
{
/* 
	DynamicLabel class just for numbers.
	Easier to use then the DynamicLabel class because
		the max size gets calculated inside this class.

	Todo: Make it work with signed numbers.
*/
class DynamicNumberLabel
	: public DynamicLabel
{
public:
	DynamicNumberLabel(unsigned maxLength, const Float3& color, float fontSize = 18);

	const std::string GetTypeName() const override { return "DynamicNumberLabel"; }

	void SetNumber(unsigned number);

private:
	using DynamicLabel::UpdateText;
	using DynamicLabel::GetText;
	unsigned m_MaxUnits; //max nr of numbers (123 => 3)

	void ToString(unsigned number, std::string& str);
};
}
}