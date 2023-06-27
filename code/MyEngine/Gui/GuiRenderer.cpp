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
	: m_InputLayout{ Rendering::InputLayout::FromType<Vertex>() }
	, m_Shader{ Resources::GlobalShader(L"Gui.hlsl") }
	, m_CanvasSize{ Globals::pCanvas->GetSize() }
{
}

void Gui::GuiRenderer::OnCanvasResize(const Int2& newSize)
{
	const Float2 scale
	{
		static_cast<float>(m_CanvasSize.x) / newSize.x,
		static_cast<float>(m_CanvasSize.y) / newSize.y
	};
	m_CanvasSize = newSize;

	for (int i = 0; i < m_Vertices.GetSize(); i += VERTICES_PER_RECT)
	{
		const Float2& pivot{ m_Pivots[i / VERTICES_PER_RECT] };
		RectFloat ndcRect{ GetNdcRect(i) };
		const Float2 pivotToLeftBot{ ndcRect.GetLeftBot() - pivot };

		ndcRect.SetLeftBot(pivot + pivotToLeftBot.Scaled(scale));
		ndcRect.SetSize(ndcRect.GetSize().Scaled(scale));
		Replace(i, ndcRect);
	}
}

void Gui::GuiRenderer::Render()
{
	m_DepthStencilState.Activate();
	m_InputLayout.Activate();
	m_Shader.Activate();
	m_Vertices.Draw();
}

//offset & size in pixels
//pivot [-1,1]
Gui::GuiRenderer::ElementId Gui::GuiRenderer::Add(const Float2& pivot, const Int2& offset, const Int2& size,
	const Float3& color)
{
	const Float2 canvasScale{ 1.f / Float::Cast(m_CanvasSize.x), 1.f / Float::Cast(m_CanvasSize.y) };
	const Float2 offsetNdc{ Float2{offset * 2}.Scaled(canvasScale) };
	const Float2 halfSizeNdc{ Float2{size}.Scaled(canvasScale) };
	const Float2 leftBotNdc{ pivot - halfSizeNdc - halfSizeNdc.Scaled(pivot) + offsetNdc };

	const int vertexIdx{ m_Vertices.GetSize() };
	Add(vertexIdx, { leftBotNdc, halfSizeNdc * 2 }, color);
	m_Pivots.Add(pivot);
	return ToElementId(vertexIdx);
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::AddCenterBottom(const Int2& offset, const Int2& size, const Float3& color)
{
	return Add({ 0,-1 }, offset, size, color);
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::GetUnderMouse() const
{
	const Float2 mouse{ MouseInClip() };
	for (int i = m_Vertices.GetSize() - VERTICES_PER_RECT; i >= 0; i -= VERTICES_PER_RECT)
	{
		const Float2& botLeft{ m_Vertices[i].pos };
		if (!mouse.IsRightAbove(botLeft)) continue;
		const Float2& topRight{ m_Vertices[i + VERTICES_PER_RECT - 1].pos };
		if (!mouse.IsLeftBelow(topRight)) continue;
		return ToElementId(i);
	}
	return ElementId{ -1 };
}

void Gui::GuiRenderer::ChangeColor(ElementId id, const Float3& color)
{
	int vertexId{ ToVertexId(id) };
	m_Vertices[vertexId++].col = color;
	m_Vertices[vertexId++].col = color;
	m_Vertices[vertexId++].col = color;

	m_Vertices[vertexId++].col = color;
	m_Vertices[vertexId++].col = color;
	m_Vertices[vertexId].col = color;
}

void Gui::GuiRenderer::ChangePositionX(ElementId id, int x)
{
	int vertexId{ ToVertexId(id) };
	const float newX = ToClipAlignCenter(x, m_CanvasSize.x);
	const float change{ newX - m_Vertices[vertexId].pos.x };

	m_Vertices[vertexId++].pos.x += change;
	m_Vertices[vertexId++].pos.x += change;
	m_Vertices[vertexId++].pos.x += change;

	m_Vertices[vertexId++].pos.x += change;
	m_Vertices[vertexId++].pos.x += change;
	m_Vertices[vertexId].pos.x += change;
}

void Gui::GuiRenderer::Add(int idx, const RectFloat& rect, const Float3& color)
{
	List<Vertex>& list{m_Vertices.GetList()};
	list.InsertEmpty(idx, VERTICES_PER_RECT);

	list[idx++] = { rect.GetLeftBot(), color };
	list[idx++] = { rect.GetLeftTop(), color };
	list[idx++] = { rect.GetRightBot(), color };

	list[idx++] = { rect.GetRightBot(), color };
	list[idx++] = { rect.GetLeftTop(), color };
	list[idx] = { rect.GetRightTop(), color };
}

void Gui::GuiRenderer::Replace(int idx, const RectFloat& rect)
{
	m_Vertices[idx++].pos = rect.GetLeftBot();
	m_Vertices[idx++].pos = rect.GetLeftTop();
	m_Vertices[idx++].pos = rect.GetRightBot();

	m_Vertices[idx++].pos = rect.GetRightBot();
	m_Vertices[idx++].pos = rect.GetLeftTop();
	m_Vertices[idx].pos = rect.GetRightTop();
}

RectFloat Gui::GuiRenderer::GetNdcRect(int idx)
{
	return {
		m_Vertices[idx].pos,
		m_Vertices[idx + VERTICES_PER_RECT - 1].pos - m_Vertices[idx].pos
	};
}

Float2 Gui::GuiRenderer::MouseInClip() const
{
	const Int2& mouseInt{ Globals::pMouse->GetPos() };
	return Float2{ mouseInt }.Divided(m_CanvasSize).Scaled({ 2,-2 }) - Float2{1, -1};
}

int Gui::GuiRenderer::ToVertexId(ElementId id)
{
	return id.GetId() * VERTICES_PER_RECT;
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::ToElementId(int vertexId)
{
	return ElementId{ vertexId / VERTICES_PER_RECT };
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
