#include "pch.h"
#include "DebugTriangles.h"

using namespace Debug;

void DebugTriangles::Render()
{
	m_Vertices.Draw();
	m_Vertices.Clear();
}

void DebugTriangles::Draw(const Triangle& triangle, const Float3& color)
{
	const Float3 normal{ triangle.FindNormal() };

	for (unsigned i{ 0 }; i < 3; ++i)
		m_Vertices.Add(Vertex{
		triangle.Points[i], color, normal });
}
