#pragma once
#include "Applied/NewUi/Elem.h"

namespace MyEngine
{
	namespace NewUi
	{
		class Label final
			: public Elem
		{
		public:
			explicit Label(const std::string& text, const Float3& color = { 0,0,0 }, float fontSize = 18);

			const std::string GetTypeName() const override { return "Label"; }

		private:
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			void Clear() override;
			void Create() override;

			std::string m_Text;
			Float3 m_Color;
			float m_FontSize;

			unsigned m_TextId{};
		};
	}
}
