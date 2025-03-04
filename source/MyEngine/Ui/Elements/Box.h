#pragma once
#include "..\Elem.h"
#include "..\GraphicsId.h"

namespace MyEngine
{
namespace Ui
{
class Box final
	: public Elem
{
public:
	explicit Box(const Float3& color);

	void TreeUpdate(const ResizePref& pref) override;
	const std::string GetTypeName() const override;

	void SetColor(const Float3& newColor);
	void MoveTo(const Float2& leftBot);

private:
	RectId m_BoxId{ };
	Float3 m_Color;

	void Clear() override;
	void Create() override;
};
}
}