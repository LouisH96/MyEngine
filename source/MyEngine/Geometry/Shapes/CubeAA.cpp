#include "pch.h"
#include "CubeAA.h"

#include <Transform\WorldMatrix.h>

using namespace MyEngine;

CubeAA::CubeAA() : m_Origin{ 0.f }, m_Size{ 0.f } {}
CubeAA::CubeAA(Float3 origin, Float3 size) : m_Origin{ origin }, m_Size{ size } {}
CubeAA::CubeAA(Float3 origin, float size) : m_Origin{ origin }, m_Size{ size } {}

Float3 CubeAA::GetRelativePoint(const Float3& alpha) const
{
	return m_Origin + alpha * m_Size;
}

bool CubeAA::Overlaps(const CubeAA& other) const
{
	const Float3 diff{ other.GetOrigin() - GetOrigin() };
	return
		((0 <= diff.x && diff.x <= m_Size.x) || (diff.x <= 0 && other.m_Size.x >= -diff.x))
		&&
		((0 <= diff.y && diff.y <= m_Size.y) || (diff.y <= 0 && other.m_Size.y >= -diff.y))
		&&
		((0 <= diff.z && diff.z <= m_Size.z) || (diff.z <= 0 && other.m_Size.z >= -diff.z));
}

CubeAA CubeAA::FromCenter(const Float3& center, const Float3& size)
{
	return CubeAA{ center - size / 2.f, size };
}

CubeAA CubeAA::FromBotCenter(const Float3& botCenter, const Float3& size)
{
	return CubeAA{ botCenter - Float3{size.x / 2.f, 0, size.z / 2.f}, size };
}

CubeAA CubeAA::MakeGlobalBounds(const Float4X4& transform, const Float3& size)
{
	Float3 minBounds{ WorldMatrix::GetPosition(transform) };
	Float3 maxBounds{ minBounds };
	const Float3 localAxes[3]{
		WorldMatrix::GetAxis(transform, 0),
		WorldMatrix::GetAxis(transform, 1),
		WorldMatrix::GetAxis(transform, 2)
	};
	for (unsigned iGlobalAxis{ 0 }; iGlobalAxis < 3; ++iGlobalAxis)
		for (unsigned iLocalAxis{ 0 }; iLocalAxis < 3; ++iLocalAxis)
		{
			const float delta{ localAxes[iLocalAxis][iGlobalAxis] };
			if (delta > 0)
				maxBounds[iGlobalAxis] += delta * size[iLocalAxis];
			else
				minBounds[iGlobalAxis] += delta * size[iLocalAxis];
		}

	return CubeAA{
		minBounds, maxBounds - minBounds
	};
}
