#pragma once

#include <d3d11.h>

namespace MyEngine
{
namespace Rendering
{
class DepthStencilBuffer
{
public:
	DepthStencilBuffer() = default;
	bool Init(const Int2& size);
	bool Update(const Int2& size);
	~DepthStencilBuffer();

	ID3D11DepthStencilView* GetView() { return m_pView; };
	const ID3D11DepthStencilView* GetView() const { return m_pView; }

private:
	static const DXGI_FORMAT TEXTURE_FORMAT{ DXGI_FORMAT_D24_UNORM_S8_UINT };
	ID3D11DepthStencilView* m_pView{ nullptr };

	static ID3D11Texture2D* MakeTexture(const Int2& size);
};
}
}
