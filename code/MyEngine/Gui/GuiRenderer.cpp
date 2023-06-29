#include "pch.h"
#include "GuiRenderer.h"

#include "Framework/Resources.h"
#include "Rendering/Canvas.h"

using namespace Rendering;

Gui::GuiRenderer::ElementId::ElementId(int id) : m_Id{ id } {}

bool Gui::GuiRenderer::ElementId::operator==(const ElementId& other) const
{
	return m_Id == other.m_Id;
}

Gui::GuiRenderer::GuiRenderer()
	: m_InputLayout{ Rendering::InputLayout::FromTypes<Vertex, Instance>() }
	, m_Shader{ Resources::GlobalShader(L"Gui.hlsl") }
	, m_InvCanvasSize{ Float2{1}.Divided(Globals::pCanvas->GetSize()) }
	, m_Instances{ 5 }
	, m_Topology{ PrimitiveTopologyUtils::ToDx(PrimitiveTopology::TriangleStrip) }
{
	Array<Vertex> vertices{4};
	vertices[0].pos = { -.5f,-.5f };
	vertices[1].pos = { -.5f,.5f };
	vertices[2].pos = { .5f,-.5f };
	vertices[3].pos = { .5f,.5f };
	m_Vertices = Buffer<Vertex>(vertices.GetData(), vertices.GetSizeU());
}

void Gui::GuiRenderer::OnCanvasResize(const Int2& newSize)
{
	Float2 scale{ m_InvCanvasSize };
	m_InvCanvasSize = Float2{ 1 }.Divided(Float2{ newSize });
	scale = m_InvCanvasSize.Divided(scale); // old/new

	Instance* pInstance{ m_Instances.GetCpuData().GetFirst() };
	for (int i = 0; i < m_Instances.GetCount(); i++, pInstance++)
	{
		if (!Instance::IsValid(*pInstance)) continue;

		const Float2& pivot{ m_Pivots[i] };

		const Float2 pivotToCenter{ pInstance->offset - pivot };
		pInstance->offset = pivot + pivotToCenter.Scaled(scale);
		pInstance->size.Scale(scale);
	}
}

void Gui::GuiRenderer::Render()
{
	m_DepthStencilState.Activate();
	m_InputLayout.Activate();
	m_Shader.Activate();

	PrimitiveTopologyUtils::Activate(m_Topology);
	m_Vertices.ActivateVertexBuffer(0);
	m_Instances.ActivateVertexBuffer(1);
	m_Instances.Draw(m_Vertices.GetCapacity());
}

void Gui::GuiRenderer::Remove(ElementId id)
{
	m_Instances.Remove(id.GetId());
}

//offset & size in pixels
//pivot [-1,1]
Gui::GuiRenderer::ElementId Gui::GuiRenderer::Add(const Float2& pivot, const Float2& offset, const Float2& size,
	const Float3& color)
{
	const Float2 offsetNdc{ (offset * 2).Scaled(m_InvCanvasSize) };
	const Float2 halfSizeNdc{ size.Scaled(m_InvCanvasSize) };
	const Float2 leftBotNdc{ pivot - halfSizeNdc.Scaled(pivot) + offsetNdc };

	const int idx{ m_Instances.Add({ leftBotNdc, halfSizeNdc * 2, color }) };
	m_Pivots.EnsureSize(idx + 1);
	m_Pivots[idx] = pivot;

	return ElementId{ idx };
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::AddCenterBottom(const Float2& offset, const Float2& size, const Float3& color)
{
	return Add({ 0,-1 }, offset, size, color);
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::GetElementUnderMouse() const
{
	const Float2 mouse{ GetMouseNdc() };

	for (int i = m_Instances.GetCpuData().GetLastIdx(); i >= m_Instances.GetCpuData().GetFirstIdx(); i--)
	{
		const Instance& instance{ m_Instances[i] };
		if (IsEmpty(instance)) continue;
		const Float2 botLeft{ instance.offset - instance.size * .5 };
		if (!mouse.IsRightAbove(botLeft)) continue;
		const Float2 topRight{ botLeft + instance.size };
		if (!mouse.IsLeftBelow(topRight)) continue;
		return ElementId{ i };
	}
	return ElementId{ -1 };
}

void Gui::GuiRenderer::SetColor(ElementId id, const Float3& color)
{
	m_Instances.GetCpuData().GetData()[id.GetId()].color = color;
}

void Gui::GuiRenderer::SetOffsetX(ElementId id, float xPixels)
{
	const Float2& pivot{ m_Pivots[id.GetId()] };
	Instance& instance{ m_Instances[id.GetId()] };

	const float localOffset{ instance.size.x * .5f * pivot.x };
	const float globalOffset{ xPixels * m_InvCanvasSize.x * 2.f };
	instance.offset.x = pivot.x - localOffset + globalOffset;
}

void Gui::GuiRenderer::SetOffsetY(ElementId id, float yPixels)
{
	const Float2& pivot{ m_Pivots[id.GetId()] };
	Instance& instance{ m_Instances[id.GetId()] };

	const float localOffset{ instance.size.y * .5f * pivot.y };
	const float globalOffset{ yPixels * m_InvCanvasSize.y * 2.f };
	instance.offset.y = pivot.y - localOffset + globalOffset;
}

Float2 Gui::GuiRenderer::GetMouseNdc() const
{
	return ScreenSpaceToNdc(Globals::pMouse->GetPos());
}

Float2 Gui::GuiRenderer::ScreenSpaceToNdc(const Int2& point) const
{
	return Float2{ point.Scaled({2,-2}) }.Scaled(m_InvCanvasSize) - Float2{1, -1};
}

Float2 Gui::GuiRenderer::ScreenSpaceToNdc(const Float2& point) const
{
	return point.Scaled(m_InvCanvasSize).Scaled({ 2,-2 }) - Float2{1, -1};
}

bool Gui::GuiRenderer::IsEmpty(const Instance& instance)
{
	return !Instance::IsValid(instance);
}

void Gui::GuiRenderer::SetEmpty(Instance& instance)
{
	Instance::Invalidate(instance);
}
