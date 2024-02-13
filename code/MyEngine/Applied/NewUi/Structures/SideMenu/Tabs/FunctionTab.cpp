#include "pch.h"
#include "FunctionTab.h"

using namespace NewUi;

FunctionTab::FunctionTab()
	: SideMenuTab{ "", nullptr }
	, m_Function{}
{
}

FunctionTab::FunctionTab(const std::string& title, Function function)
	: SideMenuTab{ title, nullptr }
	, m_Function{ function }
{
}

FunctionTab::FunctionTab(const std::string& title, SideMenuTab& parent, Function function)
	: SideMenuTab{ title, &parent }
	, m_Function{ function }
{
}

void FunctionTab::SetFunction(Function function)
{
	m_Function = function;
}

void FunctionTab::Generate(ListElem& parent)
{
	m_Function(parent);
}
