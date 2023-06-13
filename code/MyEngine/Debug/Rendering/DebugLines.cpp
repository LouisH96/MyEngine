#include "pch.h"
#include "DebugLines.h"

#include "Framework/Resources.h"

DebugLines::DebugLines(const Rendering::Gpu& gpu)
	: m_InputLayout{ Vertex::ELEMENTS, Vertex::NR_ELEMENTS }
	, m_Shader{ Framework::Resources::GlobalShader(L"unlit.hlsl") }
	, m_VertexList{ 5, Rendering::PrimitiveTopology::LineList }
{

}

void DebugLines::Render(const Rendering::Gpu& gpu)
{
	m_InputLayout.Activate();
	m_Shader.Activate();
	m_VertexList.Draw();
	m_VertexList.Clear();
}

void DebugLines::DrawLine(const Float3& begin, const Float3& end, const Float3& color)
{
	m_VertexList.Add({ begin, color });
	m_VertexList.Add({ end, color });
}

void DebugLines::DrawRay(const Float3& origin, const Float3& displacement, const Float3& color)
{
	m_VertexList.Add({ origin, color });
	m_VertexList.Add({ origin + displacement, color });
}
