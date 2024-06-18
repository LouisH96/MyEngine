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
			~SamplerState();
			SamplerState(const SamplerState& other) = delete;
			SamplerState(SamplerState&& other) noexcept = delete;
			SamplerState& operator=(const SamplerState& other) = delete;
			SamplerState& operator=(SamplerState&& other) noexcept = delete;

			void ActivateVs() const;
			void ActivatePs() const;
			void Activate() const;

		private:
			ID3D11SamplerState* m_pSamplerState{};
		};
	}
}
