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

			void Create();

			NewUiFontRenderer& GetFontRenderer() { return m_FontRenderer; }

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
