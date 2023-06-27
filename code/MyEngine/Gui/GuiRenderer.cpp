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
		static_cast<float>(m_CanvasSize.x) / newSize.x,
		static_cast<float>(m_CanvasSize.y) / newSize.y
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
Gui::GuiRenderer::ElementId Gui::GuiRenderer::Add(const Float2& pivot, const Int2& offset, const Int2& size,
	const Float3& color)
{
	const Float2 canvasScale{ 1.f / Float::Cast(m_CanvasSize.x), 1.f / Float::Cast(m_CanvasSize.y) };
	const Float2 offsetNdc{ Float2{offset * 2}.Scaled(canvasScale) };
	const Float2 halfSizeNdc{ Float2{size}.Scaled(canvasScale) };

	const Float2 leftBotNdc{ pivot - halfSizeNdc.Scaled(pivot) + offsetNdc };

	const int vertexIdx{ static_cast<int>(m_Instances.GetSize()) };
	AddNdc(vertexIdx, { leftBotNdc, halfSizeNdc * 2 }, color);
	m_Pivots.Add(pivot);
	return ElementId{ vertexIdx };
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::AddCenterBottom(const Int2& offset, const Int2& size, const Float3& color)
{
	return Add({ 0,-1 }, offset, size, color);
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::GetUnderMouse() const
{
	const Float2 mouse{ MouseInClip() };
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

void Gui::GuiRenderer::ChangeColor(ElementId id, const Float3& color)
{
	m_Instances[id.GetId()].color = color;
}

void Gui::GuiRenderer::SetOffsetX(ElementId id, int xPixels)
{
	const Float2& pivot{ m_Pivots[id.GetId()] };
	Instance& instance{ m_Instances[id.GetId()] };

	const float localOffset{ instance.size.x * .5f * pivot.x };
	const float globalOffset{ Float::Cast(xPixels) / m_CanvasSize.x * 2.f };

	instance.offset.x = pivot.x - localOffset + globalOffset;
}

void Gui::GuiRenderer::AddNdc(int idx, const RectFloat& rect, const Float3& color)
{
	m_Instances.Insert({
		rect.GetLeftBot(), rect.GetSize(), color
		}, idx);
}

Float2 Gui::GuiRenderer::MouseInClip() const
{
	const Int2& mouseInt{ Globals::pMouse->GetPos() };
	return Float2{ mouseInt }.Divided(m_CanvasSize).Scaled({ 2,-2 }) - Float2{1, -1};
}

float Gui::GuiRenderer::ToClipAlignMin(int screenPos, float screenSize)
{
	return static_cast<float>(screenPos) / screenSize * 2 - 1;
}

float Gui::GuiRenderer::ToClipAlignCenter(int screenPos, float screenSize)
{
	return static_cast<float>(screenPos) / screenSize * 2;
}

float Gui::GuiRenderer::SizeToClip(int size, float screenSize)
{
	return static_cast<float>(size) / screenSize * 2;
}

Float2 Gui::GuiRenderer::SizeToClip(const Int2& size, const Float2& screenSize)
{
	return {
		static_cast<float>(size.x) / screenSize.x * 2,
		static_cast<float>(size.y) / screenSize.y * 2
	};
}

Float2 Gui::GuiRenderer::GetPivotPoint(const Float2& pivot, const RectFloat& rect)
{
	return rect.GetLeftBot() + rect.GetSize().Scaled((pivot + 1) / 2);
}

Float2 Gui::GuiRenderer::GetLeftBottom(const Float2& pivot, const Float2& pivotPos, const RectFloat& rect)
{
	return (pivotPos - (pivot + 1) / 2).Scaled(rect.GetSize());
}
