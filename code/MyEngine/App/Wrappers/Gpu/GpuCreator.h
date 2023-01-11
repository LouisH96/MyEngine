#pragma once

namespace MyEngine
{
	namespace App
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
}

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Gpu
			{
				class GpuCreator
				{
				public:
					static Gpu* Create(Win32::Window& window);
				};
			}
		}
	}
}

