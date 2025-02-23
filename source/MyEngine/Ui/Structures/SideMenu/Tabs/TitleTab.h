#pragma once
#include "SideMenuTab.h"

namespace MyEngine
{
namespace Ui
{
class TitleTab final : public SideMenuTab
{
public:
	explicit TitleTab(const std::string& title, SideMenuTab* pParent);

	void Generate(ListElem& parent) override;
};
}
}
