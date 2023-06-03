#include "pch.h"
#include "GuiRenderer.h"

#include "Framework/Resources.h"
#include "Math/RectFloat.h"
#include "Rendering/Canvas.h"

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

	for (int i = 0; i < m_Vertices.GetSize();)
	{
		RectFloat rect
		{
			(m_Vertices[i].pos + Float2{1,1}) / 2,
			m_Vertices[i + 5].pos - m_Vertices[i].pos 
		};
		rect.SetLeft(rect.GetLeft() * scale.x * 2 - 1);
		rect.SetBottom(rect.GetBottom() * scale.y * 2 - 1);
		rect.SetWidth(rect.GetWidth() * scale.x );
		rect.SetHeight(rect.GetHeight() * scale.y );

		m_Vertices[i++].pos = rect.GetLeftBot();
		m_Vertices[i++].pos = rect.GetLeftTop();
		m_Vertices[i++].pos = rect.GetRightBot();

		m_Vertices[i++].pos = rect.GetRightBot();
		m_Vertices[i++].pos = rect.GetLeftTop();
		m_Vertices[i++].pos = rect.GetRightTop();
	}
	m_CanvasSize = newSize;
}

void Gui::GuiRenderer::Render()
{
	m_InputLayout.Activate(*Globals::pGpu);
	m_Shader.Activate();
	m_Vertices.Draw();
}

void Gui::GuiRenderer::Add(const RectInt& rectangle, const Float3& color)
{
	const RectFloat rectScaled
	{
		{
			static_cast<float>(rectangle.GetLeft()) / m_CanvasSize.x * 2 - 1,
			static_cast<float>(rectangle.GetBottom()) / m_CanvasSize.y * 2 - 1
		},
		{
			static_cast<float>(rectangle.GetWidth()) / (m_CanvasSize.x / 2),
			static_cast<float>(rectangle.GetHeight()) / (m_CanvasSize.y / 2)
		}
	};

	m_Vertices.Add({ rectScaled.GetLeftBot(), color });
	m_Vertices.Add({ rectScaled.GetLeftTop(), color });
	m_Vertices.Add({ rectScaled.GetRightBot(), color });

	m_Vertices.Add({ rectScaled.GetRightBot(), color });
	m_Vertices.Add({ rectScaled.GetLeftTop(), color });
	m_Vertices.Add({ rectScaled.GetRightTop(), color });
}
