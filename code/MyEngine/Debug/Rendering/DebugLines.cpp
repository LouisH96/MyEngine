#include "pch.h"
#include "DebugLines.h"

#include "Framework/Resources.h"

DebugLines::DebugLines(const Rendering::Gpu& gpu)
	: m_InputLayout{ gpu, Vertex::ELEMENTS, Vertex::NR_ELEMENTS }
	, m_Shader{ gpu, Framework::Resources::GetGlobalShaderPath(L"unlit.hlsl") }
	, m_VertexBuffer{ gpu, sizeof(Vertex), 6, false, Rendering::PrimitiveTopology::LineList }
{

}

void DebugLines::Render(const Rendering::Gpu& gpu)
{
	if (m_Points.IsEmpty()) return;
	m_VertexBuffer.EnsureCapacity(gpu, m_Points.GetSize(), false);
	m_VertexBuffer.UpdateData(gpu, m_Points.GetData(), m_Points.GetSize());
	m_VertexBuffer.SetCount(m_Points.GetSize());
	m_Points.Clear();

	m_InputLayout.Activate(gpu);
	m_Shader.Activate();
	m_VertexBuffer.Activate(gpu);
	m_VertexBuffer.Draw(gpu);
}

void DebugLines::DrawLine(const Float3& begin, const Float3& end, const Float3& color)
{
	m_Points.Add({ begin, color });
	m_Points.Add({ end, color });
}

void DebugLines::DrawRay(const Float3& origin, const Float3& displacement, const Float3& color)
{
	m_Points.Add({ origin, color });
	m_Points.Add({ origin + displacement, color });
}
