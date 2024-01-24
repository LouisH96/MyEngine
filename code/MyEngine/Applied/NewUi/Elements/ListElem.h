#pragma once
#include "Applied/NewUi/EmptyChildOptions.h"
#include "Applied/NewUi/ParentElem.h"

namespace MyEngine
{
	namespace NewUi
	{
		class ListElem final
			: public ParentElem<EmptyChildOptions>
		{
		public:
			ListElem(const Float2& borderMargin, float childMargin);

			void UpdateSizeAndTreePositions(const ResizePref& pref) override;

		private:
			Float2 m_BorderMargin{};
			float m_ChildMargin;

			void Clear() override;
			void Create() override;

		};
	}
}
