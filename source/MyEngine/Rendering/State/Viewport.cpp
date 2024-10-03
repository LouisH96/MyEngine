#include "pch.h"
#include "Viewport.h"

using namespace Rendering;

Viewport::Viewport()
	: m_Viewport{
		0,0,
		1,1,
		0,1 }
{
}

Viewport::Viewport(const Float2& size, const Float2& leftTop)
	: m_Viewport{
		leftTop.x, leftTop.y,
		size.x, size.y,
		0,1
	}
{
}

void Viewport::Activate() const
{
	Globals::pGpu->GetContext().RSSetViewports(1, &m_Viewport);
}

void Viewport::SetLeftTop(const Float2& leftTop)
{
	m_Viewport.TopLeftX = leftTop.x;
	m_Viewport.TopLeftY = leftTop.y;
}

void Viewport::SetSize(const Float2& size)
{
	m_Viewport.Width = size.x;
	m_Viewport.Height = size.y;
}
