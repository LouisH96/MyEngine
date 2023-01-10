#pragma once

namespace MyEngine
{
	namespace App
	{
		class Window;
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
			static IGpu* Create(App::Window& window);
		};
	}
}

