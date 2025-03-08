#pragma once

#include "..\Elem.h"
#include <Ui\GraphicsId.h>

namespace MyEngine
{
namespace Ui
{
/*
	A label Element that can be changed frequently (like each frame),
		but does not trigger a TreeUpdate.

	Since it does not trigger a TreeUpdate, enough space for the text should
		be ensured in some other way.
		For example: adding it in an Extender with the right size.
		Todo: This shouldn't be the case. This class should have the max possible
			size & also have the functions to figure that size out.
*/
class DynamicLabel
	: public Elem
{
public:
	DynamicLabel(const std::string& text, const Float2& size, const Float3& color, float fontSize = 18);

	const std::string GetTypeName() const override { return "DynamicLabel"; }

	std::string& GetText() { return m_Text; }
	void UpdateText();

	void SetPivot(const Float2& pivot);
	void SetColor(const Float3& color);

protected:
	std::string m_Text;
	Float3 m_Color;
	float m_FontSize;
	Float2 m_Pivot;
	FontId m_TextId{};

	void TreeUpdate(const ResizePref& pref) override;
	void Clear() override;
	void Create() override;
};
}
}