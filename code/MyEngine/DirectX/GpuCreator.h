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
	namespace DirectX
	{
		class DxDevice;

		class GpuCreator
		{
		public:
			static DxDevice* Create(App::IWindow& window);
		};
	}
}

