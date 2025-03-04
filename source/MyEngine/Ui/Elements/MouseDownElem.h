#pragma once
#include "..\Elem.h"

namespace MyEngine
{
namespace Ui
{
class MouseDownElem
	: public Elem
{
public:
	MouseDownElem();

	const std::string GetTypeName() const override { return "MouseDownElem"; }
	void TreeUpdate(const ResizePref& pref) override;

	void OnMouseDown(Float2 relativePos) override;

	bool IsMouseDown(); //clears flag
	const Float2& GetMouseRelPos() const { return m_RelPos; }

private:
	bool m_IsMouseDown{ false };
	Float2 m_RelPos{}; //relative to elem

	void Clear() override {};
	void Create() override {};
};
}
}