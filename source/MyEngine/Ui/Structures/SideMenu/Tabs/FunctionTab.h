#pragma once
#include "SideMenuTab.h"
#include <string>

namespace MyEngine
{
namespace Ui
{
template<typename Data>
class FunctionTab final : public SideMenuTab
{
public:
	using Function = void (*)(ListElem&, Data&);
	FunctionTab();
	explicit FunctionTab(const std::string& title, SideMenuTab* pParent, Data& data, Function function);
	explicit FunctionTab(const std::string& title, SideMenuTab* pParent, Function function);

	void Generate(ListElem& parent) override;

private:
	Function m_Function;
	Data* m_pData;
};

template <typename Data>
FunctionTab<Data>::FunctionTab()
	: SideMenuTab{ "", nullptr }
	, m_Function{}
	, m_pData{ nullptr }
{
}

template <typename Data>
FunctionTab<Data>::FunctionTab(const std::string& title, SideMenuTab* pParent, Data& data, Function function)
	: SideMenuTab{ title, pParent }
	, m_Function{ function }
	, m_pData{ &data }
{
}

template<typename Data>
inline FunctionTab<Data>::FunctionTab(const std::string& title, SideMenuTab* pParent, Function function)
	: SideMenuTab{ title, pParent }
	, m_Function{ function }
	, m_pData{ nullptr }
{
}

template <typename Data>
void FunctionTab<Data>::Generate(ListElem& parent)
{
	m_Function(parent, *m_pData);
}
}
}
