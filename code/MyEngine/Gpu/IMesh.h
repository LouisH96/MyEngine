#pragma once

namespace MyEngine
{
	namespace Gpu
	{
		class IMesh
		{
		public:
			IMesh(const IMesh& other) = delete;
			IMesh(IMesh&& other) noexcept = delete;
			IMesh& operator=(const IMesh& other) = delete;
			IMesh& operator=(IMesh&& other) noexcept = delete;

			IMesh() = default;
			virtual ~IMesh() = default;

			virtual void Draw() const = 0;
		};
	}
}