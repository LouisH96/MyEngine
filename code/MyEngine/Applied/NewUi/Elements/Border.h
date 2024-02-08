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
			explicit Border(bool shrink);
			const std::string GetTypeName() const override { return "Border"; }

		private:
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			void Clear() override;
			void Create() override;
			bool m_Shrink;

			unsigned m_BorderId{};
			unsigned m_BackgroundId{};
		};
	}
}
