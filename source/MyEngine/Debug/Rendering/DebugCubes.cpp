#include "pch.h"
#include "DebugCubes.h"

#include "Generation/CubeGenerator.h"

using namespace Generation;

DebugCubes::DebugCubes()
{
}

void DebugCubes::Render()
{
	m_Vertices.Draw();
	m_Vertices.Clear();
}

void DebugCubes::Draw(const Cube& cube, const Float3& color)
{
	List<Vertex> vertices{};
	List<int> indices{};
	CubeGenerator::Generate(cube, color, vertices, indices);
	m_Vertices.Add(
		vertices.GetData(), vertices.GetSize(),
		indices.GetData(), indices.GetSize());
}

void DebugCubes::Draw(const CubeAA& cube, const Float3& color)
{
	List<Vertex> vertices{};
	List<int> indices{};
	CubeGenerator::Generate(cube, color, vertices, indices);
	m_Vertices.Add(
		vertices.GetData(), vertices.GetSize(),
		indices.GetData(), indices.GetSize());
}
