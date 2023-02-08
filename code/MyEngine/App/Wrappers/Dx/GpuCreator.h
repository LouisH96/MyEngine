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
			namespace Dx
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
			namespace Dx
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

