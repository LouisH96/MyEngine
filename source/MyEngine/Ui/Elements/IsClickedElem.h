#pragma once

#include "..\Elem.h"

namespace MyEngine
{
namespace Ui
{
class IsClickedElem
	: public Elem
{
public:
	IsClickedElem();

	bool IsClicked(); //also clears the flag

	void OnClick() override;
	const std::string GetTypeName() const override { return "IsClickedElem"; }

private:
	bool m_IsClicked{ false }; //stays true until checked

	void TreeUpdate(const ResizePref& pref) override;
	void Clear() override {};
	void Create() override {};
};
}
}