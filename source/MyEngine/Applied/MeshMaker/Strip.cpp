#include "pch.h"
#include "Strip.h"

using namespace MeshMaker;

void Strip::Add(Vertex bottom, Vertex top)
{
	m_Edges.Add({ bottom, top });
}

void Strip::AddNormal(const Float3& normal)
{
	m_Normals.Add(normal);
}
