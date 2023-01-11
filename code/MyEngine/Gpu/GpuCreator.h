#pragma once

namespace MyEngine
{
	namespace App
	{
		namespace Win32
		{
			class Window;
		}
	}
}

namespace MyEngine
{
	namespace Gpu
	{
		class IGpu;

		class GpuCreator
		{
		public:
			static IGpu* Create(App::Win32::Window& window);
		};
	}
}

