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

	void UpdateSizeAndTreePositions(const ResizePref& pref) override;
	const std::string GetTypeName() const override;

private:
	RectId m_BoxId{ };
	Float3 m_Color;

	void Clear() override;
	void Create() override;
};
}
}