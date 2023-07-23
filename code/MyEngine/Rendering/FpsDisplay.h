#pragma once

namespace MyEngine
{
	namespace Rendering
	{
		class FpsDisplay
		{
		public:
			FpsDisplay();
			
			void Render();
			void SetFps(int fps) { m_Fps = fps; }

		private:
			int m_Fps{};
			int m_TextId{};

			void SetText();
		};
	}
}
