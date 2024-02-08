#pragma once
#include "Applied/NewUi/EmptyChildOptions.h"
#include "Applied/NewUi/ParentElem.h"

namespace MyEngine
{
	namespace NewUi
	{
		class Margin final : public ParentElem<EmptyChildOptions>
		{
		public:
			explicit Margin(float margin);
			const std::string GetTypeName() const override { return "Margin"; }

		private:
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			void Clear() override;
			void Create() override;

			float m_Margin;
		};
	}
}
