#pragma once

struct ID3D11BlendState;

namespace MyEngine
{
	namespace Rendering
	{
		class Gpu;

		class BlendState
		{
		public:
			BlendState();
			BlendState(const Gpu& gpu);
			~BlendState();
			BlendState(const BlendState& other) = delete;
			BlendState(BlendState&& other) noexcept = delete;
			BlendState& operator=(const BlendState& other) = delete;
			BlendState& operator=(BlendState&& other) noexcept = delete;

			void Activate(const Gpu& gpu) const;

		private:
			ID3D11BlendState* m_pBlendState{};
		};
	}
}

