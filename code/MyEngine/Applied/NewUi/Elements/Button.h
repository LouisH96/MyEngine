#pragma once
#include "Applied/NewUi/Elem.h"

namespace MyEngine
{
	namespace NewUi
	{
		class Button final
			: public Elem
		{
		public:
			explicit Button(const std::string& text, float fontSize = 18);

			void ToDefaultState() override;
			void ToHoverState() override;
			void ToPressedState() override;
			void OnClick() override;

		private:
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			void Clear() override;
			void Create() override;

			const std::string GetTypeName() const override;

			static constexpr float BORDER_THICKNESS = 4.f;
			static const Float2 MARGIN;

			std::string m_Text;
			float m_FontSize;
			unsigned m_BorderId{};
			unsigned m_BackgroundId{};
			unsigned m_TextId{};
		};
	}
}
