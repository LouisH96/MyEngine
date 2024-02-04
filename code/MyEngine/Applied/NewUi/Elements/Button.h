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
			using FunctionArg = unsigned;
			using Function = void (*)(FunctionArg);

			explicit Button(const std::string& text, Function function, FunctionArg functionArg, float fontSize = 18);

			void ToDefaultState() override;
			void ToHoverState() override;
			void ToPressedState() override;
			void OnClick() override;

		private:
			void UpdateSizeAndTreePositions(const ResizePref& pref) override;
			void Clear() override;
			void Create() override;

			const std::string GetTypeName() const override;

			static const Float2 MARGIN;

			std::string m_Text;
			float m_FontSize;
			unsigned m_BorderId{};
			unsigned m_BackgroundId{};
			unsigned m_TextId{};
			Function m_Function;
			FunctionArg m_FunctionArg;
		};
	}
}
