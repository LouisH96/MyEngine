#include "pch.h"
#include "DebugLines.h"

#include "Framework/Resources.h"
#include "Geometry/Shapes/Line.h"

Debug::DebugLines::DebugLines()
	: m_InputLayout{ Vertex::ELEMENTS, Vertex::NR_ELEMENTS }
	, m_Shader{ Resources::GlobalShader(L"unlit.hlsl") }
	, m_VertexList{ 5, Rendering::PrimitiveTopology::LineList }
{

}

void Debug::DebugLines::Render()
{
	m_InputLayout.Activate();
	m_Shader.Activate();
	m_VertexList.Draw();
	m_VertexList.Clear();
}

void Debug::DebugLines::DrawLine(const Float3& begin, const Float3& end, const Float3& color)
{
	m_VertexList.Add({ begin, color });
	m_VertexList.Add({ end, color });
}

void DebugLines::DrawLine(const Line& line, const Float3& color)
{
	DrawLine(line.a, line.b, color);
}

void DebugLines::DrawLine(const Float3* pData, unsigned count, const Float3& color)
{
	for (unsigned i = 0; i + 1 < count; i++)
	{
		m_VertexList.Add({ pData[i], color });
		m_VertexList.Add({ pData[i + 1], color });
	}
}

void Debug::DebugLines::DrawRay(const Float3& origin, const Float3& displacement, const Float3& color)
{
	m_VertexList.Add({ origin, color });
	m_VertexList.Add({ origin + displacement, color });
}

void DebugLines::DrawRay(const Ray& ray, const Float3& color)
{
	DrawRay(ray.Origin, ray.Direction * ray.Length, color);
}
