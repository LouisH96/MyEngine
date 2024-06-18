#pragma once

struct ID3D11BlendState;

namespace MyEngine
{
	namespace Rendering
	{
		class BlendState
		{
		public:
			BlendState();
			~BlendState();
			BlendState(const BlendState& other) = delete;
			BlendState(BlendState&& other) noexcept = delete;
			BlendState& operator=(const BlendState& other) = delete;
			BlendState& operator=(BlendState&& other) noexcept = delete;

			void Activate() const;

		private:
			ID3D11BlendState* m_pBlendState{};
		};
	}
}
