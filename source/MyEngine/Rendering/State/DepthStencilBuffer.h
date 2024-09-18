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
	bool Init(const Int2& size, bool asShaderResource = false);
	bool Update(const Int2& size);
	void Clear();
	~DepthStencilBuffer();

	ID3D11DepthStencilView* GetView() { return m_pView; };
	const ID3D11DepthStencilView* GetView() const { return m_pView; }

private:
	ID3D11DepthStencilView* m_pView{ nullptr };

	static ID3D11Texture2D* MakeTexture(const Int2& size, bool asShaderResource);
};
}
}
