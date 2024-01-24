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

		private:
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			void Clear() override;
			void Create() override;

			static const Float3 COLOR_MAIN;
			static const Float3 COLOR_SECOND;
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
