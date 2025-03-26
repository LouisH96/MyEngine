#pragma once

#include <Rendering\Structs\VertexTypes.h>
#include <Rendering\DrawData\VertexList.h>
#include <Geometry\Shapes\Triangle.h>

namespace MyEngine
{
namespace Debug
{
class DebugTriangles
{
public:

	void Render();
	void Draw(const Triangle& triangle, const Float3& color);
	void Draw(const Float3* pTriangle, const Float3& color);

private:
	using Vertex = Rendering::V_PosColNorm;
	Rendering::VertexList<Vertex> m_Vertices;
};
}
}