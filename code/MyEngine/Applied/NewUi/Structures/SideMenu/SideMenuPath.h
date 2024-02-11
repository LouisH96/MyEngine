#pragma once
#include "Applied/NewUi/EmptyChildOptions.h"
#include "Applied/NewUi/ParentElem.h"

namespace MyEngine
{
	namespace NewUi
	{
		class SideMenuPath final : public ParentElem<EmptyChildOptions>
		{
		public:
			SideMenuPath();
			const std::string GetTypeName() const override { return "SideMenuPath"; }

			void Update();
			void AddChild(const std::string& title);

		private:
			struct ButtonInfo
			{
				std::string Text;
				unsigned BackgroundId;
				unsigned TextId;
				unsigned ArrowId{ Uint::MAX };

				Float2 Size;
				Float2 Pos;
				bool DisplayPoints{ false };
			};

			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			void Clear() override;
			void Create() override;

			using ParentElem::AddChild;

			static constexpr float FONT_SIZE{ 10 };
			static const Float2 BUTTON_MARGIN;
			static constexpr float ARROW_MARGIN{ 5 };
			static constexpr float ROW_MARGIN{ 4 };

			List<ButtonInfo> m_ButtonInfo{};
			Float2 m_ArrowSize;
			Float2 m_PointsSize;

			friend class SideMenuPathHelper;
		};
	}
}
