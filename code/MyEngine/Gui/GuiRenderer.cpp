#include "pch.h"
#include "GuiRenderer.h"

#include "Framework/Resources.h"
#include "Rendering/Canvas.h"

using namespace Rendering;
using namespace Gui;

GuiRenderer::GuiRenderer()
	: m_Renderer{ Resources::GlobalShader(L"Gui.hlsl") }
{
}

void GuiRenderer::OnCanvasResized(const App::ResizedEvent& event)
{
	m_Renderer.OnCanvasResized(event);
}

void GuiRenderer::Render()
{
	m_Renderer.Render();
}

void GuiRenderer::Remove(int id)
{
	m_Renderer.Remove(id);
}

int GuiRenderer::Add(const Float2& pivot, const Float2& offset, const Float2& size,
	const Float3& color)
{
	const int idx{ m_Renderer.Add(pivot, offset, size) };
	m_Renderer.Get(idx).color = color;
	return idx;
}

int GuiRenderer::AddCenterBottom(const Float2& offset, const Float2& size, const Float3& color)
{
	return Add({ 0,-1 }, offset, size, color);
}

int GuiRenderer::GetHovered() const
{
	return m_Renderer.GetElementUnderMouse();
}

int GuiRenderer::GetClicked() const
{
	if (MOUSE.IsLeftBtnPressed())
		return GetHovered();
	return -1;
}

bool GuiRenderer::IsUnderMouse(int id) const
{
	const Float2 mouse{ m_Renderer.GetMouseNdc() };
	const Instance element{ m_Renderer.Get(id) };
	const Float2 halfSize{ element.GetSize() / 2 };
	const Float2 leftBot{ element.GetCenter() - halfSize };
	const Float2 rightTop{ element.GetCenter() + halfSize };

	return mouse.IsRightAbove(leftBot)
		&& mouse.IsLeftBelow(rightTop);
}

void GuiRenderer::SetColor(int id, const Float3& color)
{
	m_Renderer.Get(id).color = color;
}

void GuiRenderer::SetOffsetX(int id, float xPixels)
{
	m_Renderer.SetOffsetX(id, xPixels);
}

void GuiRenderer::SetOffsetY(int id, float yPixels)
{
	m_Renderer.SetOffsetY(id, yPixels);
}
