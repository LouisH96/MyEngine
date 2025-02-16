#pragma once
#include "Applied/NewUi/Elem.h"

namespace MyEngine
{
namespace NewUi
{
class Box final
	: public Elem
{
public:
	explicit Box(const Float3& color);

	void UpdateSizeAndTreePositions(const ResizePref& pref) override;
	const std::string GetTypeName() const override;

private:
	unsigned m_BoxId{ Scalar<unsigned>::INVALID };
	Float3 m_Color;

	void Clear() override;
	void Create() override;
};
}
}
