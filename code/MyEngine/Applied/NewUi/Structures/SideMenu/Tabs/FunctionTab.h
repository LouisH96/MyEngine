#pragma once
#include "SideMenuTab.h"

namespace MyEngine
{
	namespace NewUi
	{
		class FunctionTab final : public SideMenuTab
		{
		public:
			using Function = void (*)(ListElem&);
			FunctionTab();
			explicit FunctionTab(const std::string& title, Function function = nullptr);
			explicit FunctionTab(const std::string& title, SideMenuTab& parent, Function function = nullptr);

			void SetFunction(Function function);
			void Generate(ListElem& parent) override;

		private:
			Function m_Function;
		};
	}
}
