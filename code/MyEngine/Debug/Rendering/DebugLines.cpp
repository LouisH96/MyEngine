#include "pch.h"
#include "DebugLines.h"

#include "DataStructures/Adders/ArrayAdder.h"
#include "Framework/Resources.h"
#include "Generation/DiskGenerator.h"
#include "Geometry/Shapes/Line.h"

using namespace Generation;

DebugLines::DebugLines()
	: m_InputLayout{ Vertex::ELEMENTS, Vertex::NR_ELEMENTS }
	, m_Shader{ Resources::GlobalShader(L"unlit.hlsl") }
	, m_VertexList{ 5, Rendering::PrimitiveTopology::LineList }
{

}

void DebugLines::Render()
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

void DebugLines::DrawLineLoop(PtrRangeConst<Float3> points, const Float3& color)
{
	m_VertexList.GetList().EnsureIncrease(points.count * 2);
	for (unsigned i = 0; i + 1 < points.count; i++)
	{
		m_VertexList.Add({ points[i], color });
		m_VertexList.Add({ points[i + 1], color });
	}
	m_VertexList.Add({ points.Last(), color });
	m_VertexList.Add({ points.First(), color });
}

void DebugLines::DrawRay(const Float3& origin, const Float3& displacement, const Float3& color)
{
	m_VertexList.Add({ origin, color });
	m_VertexList.Add({ origin + displacement, color });
}

void DebugLines::DrawRay(const Ray& ray, const Float3& color)
{
	DrawRay(ray.Origin, ray.Direction * ray.Length, color);
}

void DebugLines::DrawDiskXz(const Float3& center, float radius, const Float3& color)
{
	using Gen = DiskGenerator<ModelTopology::TriangleListIdx>;
	const Gen::Options options{ radius };

	Array<Float3> points{ Gen::GetNrVertices(options) };
	Gen::Generate([center](const Float2& point)
		{
			return Float3{ point.x + center.x, center.y, point.y + center.z };
		}, ArrayAdder<Float3>{points, 0}, options);

	DrawLineLoop(PtrRangeConst<Float3>{points}, color);
}
