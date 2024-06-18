#include "pch.h"
#include "DebugCones.h"

#include "Generation/ConeGenerator.h"

using namespace Generation;

DebugCones::DebugCones()
{
}

void DebugCones::Render()
{
	m_Vertices.Draw();
	m_Vertices.Clear();
}

void DebugCones::Draw(const Cone& cone, const Float3& color)
{
	List<Vertex> vertices{};
	List<int> indices{};
	ConeGenerator::Generate(cone, color, 16, vertices, indices);
	m_Vertices.Add(
		vertices.GetData(), vertices.GetSize(),
		indices.GetData(), indices.GetSize()
	);
}
