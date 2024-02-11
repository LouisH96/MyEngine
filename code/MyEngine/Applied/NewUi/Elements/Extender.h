#pragma once
#include "Applied/NewUi/EmptyChildOptions.h"
#include "Applied/NewUi/ParentElem.h"
#include "Applied/NewUi/SizeDef.h"

namespace MyEngine
{
	namespace NewUi
	{
		class Extender final : public ParentElem<EmptyChildOptions>
		{
		public:
			explicit Extender(const SizeDef& sizeDef);
			const std::string GetTypeName() const override { return "Extender"; }
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;

			void SetValueX(float valueX) { m_SizeDef.Value.x = valueX; }

		private:
			void Clear() override;
			void Create() override;

			SizeDef m_SizeDef;
		};
	}
}
