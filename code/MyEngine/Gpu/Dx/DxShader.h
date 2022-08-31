#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

#include "../IShader.h"

namespace MyEngine
{
	namespace Gpu
	{
		namespace Dx
		{
			class DxDevice;

			class DxShader final
				: public IShader
			{
			public:
				DxShader(const DxShader& other) = delete;
				DxShader(DxShader&& other) noexcept = delete;
				DxShader& operator=(const DxShader& other) = delete;
				DxShader& operator=(DxShader&& other) noexcept = delete;

				explicit DxShader(DxDevice& device);
				~DxShader() override;

				void Activate() const;

				struct Vertex
				{
					DirectX::XMFLOAT3 pos{};
					DirectX::XMFLOAT3 col{};
				};

			private:
				DxDevice& m_Gpu;

				ID3D11VertexShader* m_pVertexShader{};
				ID3D11PixelShader* m_pPixelShader{};
				ID3D11InputLayout* m_pInputLayout{};

				void InitShaders();
				void InitInputLayout();
			};
		}
	}
}

