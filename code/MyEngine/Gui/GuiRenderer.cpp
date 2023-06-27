#include "pch.h"
#include "GuiRenderer.h"

#include "Framework/Resources.h"
#include "Rendering/Canvas.h"

Gui::GuiRenderer::ElementId::ElementId(int id) : m_Id{ id } {}

bool Gui::GuiRenderer::ElementId::operator==(const ElementId& other) const
{
	return m_Id == other.m_Id;
}

Gui::GuiRenderer::GuiRenderer()
	: m_InputLayout{ Rendering::InputLayout::FromTypes<Vertex, Instance>() }
	, m_Shader{ Resources::GlobalShader(L"Gui.hlsl") }
	, m_CanvasSize{ Globals::pCanvas->GetSize() }
{
	Array<Vertex> vertices{4};
	vertices[0].pos = { -.5f,-.5f };
	vertices[1].pos = { -.5f,.5f };
	vertices[2].pos = { .5f,-.5f };
	vertices[3].pos = { .5f,.5f };
	m_Instances = { vertices.GetData(), vertices.GetSizeU(), Rendering::PrimitiveTopology::TriangleStrip };
}

void Gui::GuiRenderer::OnCanvasResize(const Int2& newSize)
{
	const Float2 scale
	{
		m_CanvasSize.x / newSize.x,
		m_CanvasSize.y / newSize.y
	};
	m_CanvasSize = newSize;

	for (int i = 0; i < m_Instances.GetSize(); i++)
	{
		const Float2& pivot{ m_Pivots[i] };
		Instance& instance{ m_Instances[i] };

		const Float2 pivotToCenter{ instance.offset - pivot };
		instance.offset = pivot + pivotToCenter.Scaled(scale);
		instance.size.Scale(scale);
	}
}

void Gui::GuiRenderer::Render()
{
	m_DepthStencilState.Activate();
	m_InputLayout.Activate();
	m_Shader.Activate();
	m_Instances.Draw();
}

//offset & size in pixels
//pivot [-1,1]
Gui::GuiRenderer::ElementId Gui::GuiRenderer::Add(const Float2& pivot, const Float2& offset, const Float2& size,
	const Float3& color)
{
	const Float2 canvasScale{ 1.f / m_CanvasSize.x, 1.f / m_CanvasSize.y };
	const Float2 offsetNdc{ (offset * 2).Scaled(canvasScale) };
	const Float2 halfSizeNdc{ size.Scaled(canvasScale) };

	const Float2 leftBotNdc{ pivot - halfSizeNdc.Scaled(pivot) + offsetNdc };

	const int vertexIdx{ static_cast<int>(m_Instances.GetSize()) };
	AddNdc(vertexIdx, { leftBotNdc, halfSizeNdc * 2 }, color);
	m_Pivots.Add(pivot);
	return ElementId{ vertexIdx };
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::AddCenterBottom(const Float2& offset, const Float2& size, const Float3& color)
{
	return Add({ 0,-1 }, offset, size, color);
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::GetElementUnderMouse() const
{
	const Float2 mouse{ GetMouseNdc() };
	for (int i = m_Instances.GetSize() - 1; i >= 0; i--)
	{
		const Float2 botLeft{ m_Instances[i].offset - m_Instances[i].size * .5 };
		if (!mouse.IsRightAbove(botLeft)) continue;
		const Float2 topRight{ botLeft + m_Instances[i].size };
		if (!mouse.IsLeftBelow(topRight)) continue;
		return ElementId{ i };
	}
	return ElementId{ -1 };
}

void Gui::GuiRenderer::SetColor(ElementId id, const Float3& color)
{
	m_Instances[id.GetId()].color = color;
}

void Gui::GuiRenderer::SetOffsetX(ElementId id, float xPixels)
{
	const Float2& pivot{ m_Pivots[id.GetId()] };
	Instance& instance{ m_Instances[id.GetId()] };

	const float localOffset{ instance.size.x * .5f * pivot.x };
	const float globalOffset{ xPixels / m_CanvasSize.x * 2.f };

	instance.offset.x = pivot.x - localOffset + globalOffset;
}

void Gui::GuiRenderer::AddNdc(int idx, const RectFloat& rect, const Float3& color)
{
	m_Instances.Insert({
		rect.GetLeftBot(), rect.GetSize(), color
		}, idx);
}

Float2 Gui::GuiRenderer::GetMouseNdc() const
{
	return ScreenSpaceToNdc(Globals::pMouse->GetPos());
}

Float2 Gui::GuiRenderer::ScreenSpaceToNdc(const Int2& point) const
{
	return Float2{ point.Scaled({2,-2}) }.Divided(m_CanvasSize) - Float2{1, -1};
}

Float2 Gui::GuiRenderer::ScreenSpaceToNdc(const Float2& point) const
{
	return point.Divided(m_CanvasSize).Scaled({ 2,-2 }) - Float2{1, -1};
}