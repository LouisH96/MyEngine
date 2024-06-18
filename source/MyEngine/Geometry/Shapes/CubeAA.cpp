#include "pch.h"
#include "CubeAA.h"

using namespace MyEngine;

CubeAA::CubeAA() : m_Origin{ 0.f }, m_Size{ 0.f } {}
CubeAA::CubeAA(Float3 origin, Float3 size) : m_Origin{ origin }, m_Size{ size } {}
CubeAA::CubeAA(Float3 origin, float size) : m_Origin{ origin }, m_Size{ size } {}

Float3 CubeAA::GetRelativePoint(const Float3& alpha) const
{
	return m_Origin + alpha * m_Size;
}

CubeAA CubeAA::FromCenter(const Float3& center, const Float3& size)
{
	return CubeAA{ center - size / 2.f, size };
}

CubeAA CubeAA::FromBotCenter(const Float3& botCenter, const Float3& size)
{
	return CubeAA{ botCenter - Float3{size.x / 2.f, 0, size.z / 2.f}, size };
}
