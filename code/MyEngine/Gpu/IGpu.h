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
		class IShader;
		class IMesh;
		class ICanvas;

		class IGpu
		{
		public:
			IGpu(const IGpu& other) = delete;
			IGpu(IGpu&& other) noexcept = delete;
			IGpu& operator=(const IGpu& other) = delete;
			IGpu& operator=(IGpu&& other) noexcept = delete;

			IGpu() = default;
			virtual ~IGpu() = default;

			virtual void Release() = 0;

			virtual ICanvas* MakeCanvas(App::IWindow& window) = 0;
			virtual IShader* MakeShader() = 0;
			virtual IMesh* MakeMesh() = 0;
		};
	}
}
