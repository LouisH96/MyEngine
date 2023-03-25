#pragma once

struct ID3D11SamplerState;

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			namespace Dx
			{
				class Gpu;
			}
		}
	}
}

namespace MyEngine
{
	namespace Dx
	{
		class SamplerState
		{
		public:
			SamplerState(const App::Wrappers::Dx::Gpu& gpu);
			~SamplerState();
			SamplerState(const SamplerState& other) = delete;
			SamplerState(SamplerState&& other) noexcept = delete;
			SamplerState& operator=(const SamplerState& other) = delete;
			SamplerState& operator=(SamplerState&& other) noexcept = delete;

			void ActivateVs(const App::Wrappers::Dx::Gpu& gpu) const;
			void ActivatePs(const App::Wrappers::Dx::Gpu& gpu) const;
			void Activate(const App::Wrappers::Dx::Gpu& gpu) const;

		private:
			ID3D11SamplerState* m_pSamplerState{};
		};
	}
}
