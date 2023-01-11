#pragma once

namespace MyEngine
{
	namespace Wrappers
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
			static IGpu* Create(Wrappers::Win32::Window& window);
		};
	}
}

