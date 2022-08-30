#pragma once

namespace MyEngine
{
	namespace App
	{
		class IWindow;
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
			static IGpu* Create(App::IWindow& window);
		};
	}
}

