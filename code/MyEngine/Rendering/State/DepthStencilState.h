#pragma once

struct ID3D11DepthStencilState;

namespace MyEngine
{
	namespace Rendering
	{
		class DepthStencilState
		{
		public:
			//---| Constructor/Destructor |---
			DepthStencilState();
			~DepthStencilState();

			//---| Move/Copy |---
			DepthStencilState(const DepthStencilState& other)  = delete;
			DepthStencilState& operator=(const DepthStencilState& other) = delete;
			DepthStencilState(DepthStencilState&& other) noexcept;
			DepthStencilState& operator=(DepthStencilState&& other) noexcept;

			//---| Functions |---
			void Activate() const;

		private:
			ID3D11DepthStencilState* m_pState{};
		};
	}
}
