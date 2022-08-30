#pragma once

namespace MyEngine
{
	namespace Gpu
	{
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

			virtual void Temp() const = 0;
		};
	}
}