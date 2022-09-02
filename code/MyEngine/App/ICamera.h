#pragma once

namespace MyEngine
{
	namespace App
	{
		class ICamera
		{
		public:
			ICamera(const ICamera& other) = delete;
			ICamera(ICamera&& other) noexcept = delete;
			ICamera& operator=(const ICamera& other) = delete;
			ICamera& operator=(ICamera&& other) noexcept = delete;

			ICamera() = default;
			virtual ~ICamera() = default;

			virtual void Update() = 0;
		};
	}
}