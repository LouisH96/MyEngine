#pragma once
#include "Applied/NewUi/EmptyChildOptions.h"
#include "Applied/NewUi/ParentElem.h"

namespace MyEngine
{
	namespace NewUi
	{
		class Border final : public ParentElem<EmptyChildOptions>
		{
		public:
			const std::string GetTypeName() const override { return "Border"; }

		private:
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			void Clear() override;
			void Create() override;

			unsigned m_BorderId{};
			unsigned m_BackgroundId{};
		};
	}
}
