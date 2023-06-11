#pragma once

struct ID3D11SamplerState;

namespace MyEngine
{
	namespace Rendering
	{
		class Gpu;

		class SamplerState
		{
		public:
			SamplerState();
			SamplerState(const Gpu& gpu);
			~SamplerState();
			SamplerState(const SamplerState& other) = delete;
			SamplerState(SamplerState&& other) noexcept = delete;
			SamplerState& operator=(const SamplerState& other) = delete;
			SamplerState& operator=(SamplerState&& other) noexcept = delete;

			void ActivateVs(const Gpu& gpu) const;
			void ActivatePs(const Gpu& gpu) const;
			void Activate(const Gpu& gpu) const;

		private:
			ID3D11SamplerState* m_pSamplerState{};
		};
	}
}
