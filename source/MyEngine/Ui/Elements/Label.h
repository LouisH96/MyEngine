#pragma once
#include "..\Elem.h"
#include "..\GraphicsId.h"

namespace MyEngine
{
namespace Ui
{
class Label final
	: public Elem
{
public:
	explicit Label(const std::string& text, float fontSize = 18);
	explicit Label(const std::string& text, const Float3& color, float fontSize = 18);

	const std::string GetTypeName() const override { return "Label"; }

private:
	void TreeUpdate(const ResizePref& pref) override;
	void Clear() override;
	void Create() override;

	std::string m_Text;
	Float3 m_Color;
	float m_FontSize;

	FontId m_TextId{};
};
}
}
