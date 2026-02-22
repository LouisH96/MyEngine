#pragma once

#include <d3d11.h>

namespace MyEngine::Rendering
{
class DepthStencilBuffer
{
public:
	struct Options
	{
		enum MultiSampling
		{
			GlobalSetting, Enabled, Disabled
		};

		MultiSampling MultiSampling{ GlobalSetting };
		bool AsShaderResource{ false };

		bool UsingMultiSampling() const;
	};

public:
	DepthStencilBuffer(Options options = {});
	bool Init(const Int2& size);
	bool Init(const Int2& size, Options options);
	bool Update(const Int2& size);
	void Clear();
	~DepthStencilBuffer();

	ID3D11DepthStencilView* GetView() { return m_pView; };
	const ID3D11DepthStencilView* GetView() const { return m_pView; }

	ID3D11ShaderResourceView* MakeShaderResourceView() const;

private:
	ID3D11DepthStencilView* m_pView{ nullptr };
	Options m_Options;

	static ID3D11Texture2D* MakeTexture(const Int2& size, Options options);
};
}
