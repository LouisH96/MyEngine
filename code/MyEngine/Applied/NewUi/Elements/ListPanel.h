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
			enum class Direction
			{
				LeftToRight = 0,
				BotUp = 1,
				RightToLeft = 2,
				TopDown = 3,
			};

			explicit ListPanel(Direction direction, float childMargin = 2.f);
			explicit ListPanel(const Float2& flowDirection, const Float2& fillDirection, float childMargin = 2.f);

			void AddChild(Elem* pChild);

		private:
			Float2 m_FlowDirection;
			Float2 m_FillDirection;
			float m_ChildMargin;

			unsigned m_DebugBg{ };

			static Float2 ToFlowDirection(Direction direction);
			static Float2 ToFillDirection(Direction direction);

			virtual const std::string GetTypeName() const override { return "ListPanel"; };
			virtual void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			virtual void Clear() override;
			virtual void Create() override;
		};
	}
}
