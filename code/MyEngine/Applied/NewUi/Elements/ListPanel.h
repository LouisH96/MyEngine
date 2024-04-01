#pragma once

#include <Applied/NewUi/ParentElem.h>
#include <Applied\NewUi\EmptyChildOptions.h>

namespace MyEngine
{
	namespace NewUi
	{
		class ListPanel
			: public ParentElem<EmptyChildOptions>
		{
		public:
			explicit ListPanel(const Float2& flowDirection, const Float2& fillDirection, float childMargin = 2.f);

			void AddChild(Elem* pChild);

		private:
			Float2 m_FlowDirection;
			Float2 m_FillDirection;
			float m_ChildMargin;

			unsigned m_DebugBg{ };

			virtual const std::string GetTypeName() const override { return "ListPanel"; };
			virtual void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			virtual void Clear() override;
			virtual void Create() override;
		};
	}
}
