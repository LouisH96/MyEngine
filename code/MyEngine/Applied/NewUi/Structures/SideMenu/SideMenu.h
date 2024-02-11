#pragma once

namespace MyEngine
{
	namespace NewUi
	{
		class Extender;

		class SideMenu
		{
		public:
			explicit SideMenu(float width);

			void Update();

		private:
			Extender* m_pExtender{};
			bool m_Dragging{ false };
		};
	}
}
