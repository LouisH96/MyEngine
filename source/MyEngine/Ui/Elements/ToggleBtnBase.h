#pragma once

#include "..\Elem.h"

namespace MyEngine
{
namespace Ui
{
class ToggleBtnBase
	: public Elem
{
public:
	ToggleBtnBase(bool isOn = false);
	const std::string GetTypeName() const override { return "ToggleBtnBase"; }

	void OnClick() override;

	bool HasChanged(); //also clears the flag
	bool IsOn() const { return m_IsOn; }

private:
	void UpdateSizeAndTreePositions(const ResizePref& pref) override;
	void Clear() override {};
	void Create() override {};

	bool m_HasChanged{ false }; //stays true until checked
	bool m_IsOn{ false };
};
}
}