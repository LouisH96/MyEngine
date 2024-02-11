#pragma once

namespace MyEngine
{
	namespace NewUi
	{
		class SideMenuPath;
		class Extender;

		class SideMenu
		{
		public:
			explicit SideMenu(float width);

			void Update();

		private:
			Extender* m_pExtender{};
			SideMenuPath* m_pPath{};
			bool m_Dragging{ false };
		};
	}
}
