#include "pch.h"
#include "GuiRenderer.h"

#include "Framework/Resources.h"
#include "Rendering/Canvas.h"

using namespace Rendering;

Gui::GuiRenderer::GuiRenderer()
	: m_Renderer{ Resources::GlobalShader(L"Gui.hlsl") }
{
}

void Gui::GuiRenderer::OnCanvasResized(const App::ResizedEvent& event)
{
	m_Renderer.OnCanvasResized(event);
}

void Gui::GuiRenderer::Render()
{
	m_Renderer.Render();
}

void Gui::GuiRenderer::Remove(int id)
{
	m_Renderer.Remove(id);
}

int Gui::GuiRenderer::Add(const Float2& pivot, const Float2& offset, const Float2& size,
	const Float3& color)
{
	const int idx{ m_Renderer.Add(pivot, offset, size) };
	m_Renderer.Get(idx).color = color;
	return idx;
}

int Gui::GuiRenderer::AddCenterBottom(const Float2& offset, const Float2& size, const Float3& color)
{
	return Add({ 0,-1 }, offset, size, color);
}

int Gui::GuiRenderer::GetHovered() const
{
	return m_Renderer.GetElementUnderMouse();
}

int Gui::GuiRenderer::GetClicked() const
{
	if (MOUSE.IsLeftBtnPressed())
		return GetHovered();
	return -1;
}

void Gui::GuiRenderer::SetColor(int id, const Float3& color)
{
	m_Renderer.Get(id).color = color;
}

void Gui::GuiRenderer::SetOffsetX(int id, float xPixels)
{
	m_Renderer.SetOffsetX(id, xPixels);
}

void Gui::GuiRenderer::SetOffsetY(int id, float yPixels)
{
	m_Renderer.SetOffsetY(id, yPixels);
}
