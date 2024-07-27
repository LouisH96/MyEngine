#include "pch.h"
#include "Arc.h"

using namespace MeshMaker;

using Vertex = Arc::Vertex;

void Arc::EnsureNrCorners(unsigned nrCorners)
{
	m_Corners.EnsureCapacity(nrCorners);
}

void Arc::AddCorner(const Vertex corner)
{
	m_Corners.Add(corner);
}

void Arc::SetCenter(const Vertex center)
{
	m_Center = center;
}

void Arc::SetNormal(const Float3& normal)
{
	m_Normal = normal;
}

void Arc::AddCorner(const Float3& corner)
{
	m_Corners.Add(MakerPointVertex{ corner });
}

void Arc::SetCenter(const Float3& center)
{
	m_Center = MakerPointVertex{center};
}

void Arc::ClearCorners()
{
	m_Corners.Clear();
}

Vertex Arc::GetCorner(unsigned i) const
{
	return m_Corners[i];
}

const Float3& Arc::GetNormal() const
{
	return m_Normal;
}
