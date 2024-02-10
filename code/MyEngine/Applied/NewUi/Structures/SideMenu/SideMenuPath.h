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

			void AddChild(const std::string& title);

		private:
			struct ButtonInfo
			{
				std::string Text;
				float Width;
				unsigned BackgroundId;
				unsigned TextId;

				unsigned ArrowId{ Uint::MAX };
			};

			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			void Clear() override;
			void Create() override;

			using ParentElem::AddChild;

			static constexpr float FONT_SIZE{ 10 };
			static const Float2 MARGIN;

			List<ButtonInfo> m_ButtonInfo{};
			float m_MaxButtonHeight{};
			float m_ArrowWidth;
			float m_ArrowVerOffset;
			float m_PointsWidth;
			float m_PointsVerOffset;
		};
	}
}
