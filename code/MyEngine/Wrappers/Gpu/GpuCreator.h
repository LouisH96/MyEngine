#pragma once

namespace MyEngine
{
	namespace Wrappers
	{
		namespace Win32
		{
			class Window;
		}
		namespace Gpu
		{
			class Gpu;
		}
	}
}

namespace MyEngine
{
	namespace Wrappers
	{
		namespace Gpu
		{
			class GpuCreator
			{
			public:
				static Gpu* Create(Wrappers::Win32::Window& window);
			};
		}
	}
}

