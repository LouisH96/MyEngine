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

	void SetNumber(unsigned number);

private:
	using DynamicLabel::UpdateText;
	using DynamicLabel::GetText;

	static void ToString(unsigned number, std::string& str);
};
}
}