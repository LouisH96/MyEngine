#pragma once
#include "NewUiFontRenderer.h"
#include "Elements/Root.h"

namespace MyEngine
{
	namespace NewUi
	{
		class Elem;

		class NewUiSystem
		{
		public:
			explicit NewUiSystem(const Float2& screenSize);

			void OnCanvasResized(const App::ResizedEvent& event);
			void Update();
			void Render();
			void AddChild(Elem* pChild);

			void RemoveAllChildren(); //try to remove this function
			//void RemoveChild(const Elem* pChild);

			void BeforeEdit();
			void AfterEdit();

			NewUiFontRenderer& GetFontRenderer() { return m_FontRenderer; }

			static const Float3 COLOR_DARK;
			static const Float3 COLOR_MEDIUM;
			static const Float3 COLOR_LIGHT;
			static constexpr float BORDER_THICKNESS{ 4.f };

		private:
			enum CurrentElemState
			{
				Hovered, Pressed
			};

			Root m_Root;
			NewUiFontRenderer m_FontRenderer;

			Elem* m_pCurrentElem;
			CurrentElemState m_CurrentElemState;
		};
	}
}
