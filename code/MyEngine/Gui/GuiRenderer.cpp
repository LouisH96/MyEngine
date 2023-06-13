#include "pch.h"
#include "GuiRenderer.h"

#include "Framework/Resources.h"
#include "Rendering/Canvas.h"

Gui::GuiRenderer::ElementId::ElementId(int id) : m_Id{id}{}

bool Gui::GuiRenderer::ElementId::operator==(const ElementId& other) const
{
	return m_Id == other.m_Id;
}

Gui::GuiRenderer::GuiRenderer()
	: m_InputLayout{ Rendering::InputLayout::FromType<Vertex>() }
	, m_Shader{ Framework::Resources::GetGlobalShaderPath(L"Gui.hlsl") }
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

	//left-bottom
	for (int i = 0; i < m_CenterBottomAnchoredIdx; i += VERTICES_PER_RECT)
	{
		const RectFloat rect
		{
			(m_Vertices[i].pos + Float2{1,1}).Scaled(scale) - Float2{1, 1},
			(m_Vertices[i + 5].pos - m_Vertices[i].pos).Scaled(scale)
		};
		Replace(i, rect);
	}
	for (int i = m_CenterBottomAnchoredIdx; i < m_Vertices.GetSize(); i += VERTICES_PER_RECT)
	{
		const RectFloat rect
		{
			{
				m_Vertices[i].pos.x * scale.x,
				(m_Vertices[i].pos.y + 1) * scale.y - 1
			},
			(m_Vertices[i + 5].pos - m_Vertices[i].pos).Scaled(scale)
		};
		Replace(i, rect);
	}
	m_CanvasSize = newSize;
}

void Gui::GuiRenderer::Render()
{
	m_DepthStencilState.Activate();
	m_InputLayout.Activate();
	m_Shader.Activate();
	m_Vertices.Draw();
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::AddLeftBottom(const RectInt& rectangle, const Float3& color)
{
	const RectFloat rectScaled
	{
		{
			ToClipAlignMin(rectangle.GetLeft(), m_CanvasSize.x),
			ToClipAlignMin(rectangle.GetBottom(), m_CanvasSize.y)
		},
		SizeToClip(rectangle.GetSize(), m_CanvasSize)
	};
	Add(m_CenterBottomAnchoredIdx, rectScaled, color);
	m_CenterBottomAnchoredIdx += VERTICES_PER_RECT;
	return ElementId{m_CenterBottomAnchoredIdx / VERTICES_PER_RECT - 1};
}

Gui::GuiRenderer::ElementId Gui::GuiRenderer::AddCenterBottom(const RectInt& rectangle, const Float3& color)
{
	const RectFloat rectFloat
	{
		{
			ToClipAlignCenter(rectangle.GetLeft(), m_CanvasSize.x),
			ToClipAlignMin(rectangle.GetBottom(), m_CanvasSize.y)
		},
		SizeToClip(rectangle.GetSize(), m_CanvasSize)
	};
	Add(m_Vertices.GetSize(), rectFloat, color);
	return ElementId{ 1000 + (m_Vertices.GetSize() - m_CenterBottomAnchoredIdx) / VERTICES_PER_RECT - 1 };
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
		return ElementId{ i >= m_CenterBottomAnchoredIdx
			? 1000 + (i - m_CenterBottomAnchoredIdx) / VERTICES_PER_RECT
			: i / VERTICES_PER_RECT };
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

Float2 Gui::GuiRenderer::MouseInClip() const
{
	const Int2& mouseInt{ Globals::pMouse->GetPos() };
	return Float2{ mouseInt }.Divided(m_CanvasSize).Scaled({ 2,-2 }) - Float2{1, -1};
}

int Gui::GuiRenderer::ToVertexId(ElementId id) const
{
	if (id.GetId() >= 1000)
		return (id.GetId() - 1000) * VERTICES_PER_RECT + m_CenterBottomAnchoredIdx;
	return id.GetId() * VERTICES_PER_RECT;
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
