#pragma once

#include <d3d11.h>

namespace MyEngine
{
namespace Rendering
{
class Viewport
{
public:
	Viewport();
	Viewport(const Float2& size, const Float2& leftTop = { 0,0 });

	void Activate() const;

	void SetLeftTop(const Float2& leftTop);
	void SetSize(const Float2& size);

private:
	D3D11_VIEWPORT m_Viewport;
};
}
}
