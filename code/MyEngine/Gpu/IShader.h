#pragma once

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class ICamera;
		}
	}
}

namespace MyEngine
{
	namespace Gpu
	{
		class IShader
		{
		public:
			IShader(const IShader& other) = delete;
			IShader(IShader&& other) noexcept = delete;
			IShader& operator=(const IShader& other) = delete;
			IShader& operator=(IShader&& other) noexcept = delete;

			IShader() = default;
			virtual ~IShader() = default;

			virtual void OnCamUpdated(Game::Camera::ICamera& camera) = 0;
		};
	}
}
