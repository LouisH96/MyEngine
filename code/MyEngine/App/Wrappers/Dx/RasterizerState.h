#pragma once

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

struct ID3D11RasterizerState;

namespace MyEngine
{
	namespace Dx
	{
		class RasterizerState
		{
		public:
			explicit RasterizerState(const App::Wrappers::Dx::Gpu& gpu, bool isWireframe = false);
			~RasterizerState();
			RasterizerState(const RasterizerState& other) = delete;
			RasterizerState(RasterizerState&& other) noexcept = delete;
			RasterizerState& operator=(const RasterizerState& other) = delete;
			RasterizerState& operator=(RasterizerState&& other) noexcept = delete;

			void Activate(const App::Wrappers::Dx::Gpu& gpu) const;

		private:
			ID3D11RasterizerState* m_pState{};
		};
	}
}
