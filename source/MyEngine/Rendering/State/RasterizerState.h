#pragma once

struct ID3D11RasterizerState;

namespace MyEngine
{
	namespace Rendering
	{
		class RasterizerState
		{
		public:
			explicit RasterizerState(bool isWireframe = false);
			~RasterizerState();
			RasterizerState(const RasterizerState& other) = delete;
			RasterizerState(RasterizerState&& other) noexcept = delete;
			RasterizerState& operator=(const RasterizerState& other) = delete;
			RasterizerState& operator=(RasterizerState&& other) noexcept = delete;

			void Activate() const;

		private:
			ID3D11RasterizerState* m_pState{};
		};
	}
}
