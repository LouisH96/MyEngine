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
struct ID3D11BlendState;

namespace MyEngine
{
	namespace Dx
	{
		class BlendState
		{
		public:
			BlendState(const App::Wrappers::Dx::Gpu& gpu);
			~BlendState();
			BlendState(const BlendState& other) = delete;
			BlendState(BlendState&& other) noexcept = delete;
			BlendState& operator=(const BlendState& other) = delete;
			BlendState& operator=(BlendState&& other) noexcept = delete;

			void Activate(const App::Wrappers::Dx::Gpu& gpu) const;

		private:
			ID3D11BlendState* m_pBlendState{};
		};
	}
}

