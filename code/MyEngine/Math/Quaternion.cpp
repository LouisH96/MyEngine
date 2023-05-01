#include "pch.h"
#include "Quaternion.h"

Math::Quaternion::Quaternion()
	: m_Real{ 0, 0, 0 }
	, m_Complex{ 1 }
{
}

Math::Quaternion::Quaternion(const Float3& real, float complex)
	: m_Real{ real }
	, m_Complex{ complex }
{
}

Math::Quaternion Math::Quaternion::FromAxis(const Float3& rotationAxis, float rotation)
{
	const float cosHalf{ cosf(rotation / 2) };
	const float sinHalf{ sinf(rotation / 2) };
	return Quaternion{ rotationAxis * sinHalf, cosHalf };
}

Math::Quaternion Math::Quaternion::FromForward(const Float3& forward)
{
	const Float3 axis{ Float3{0,0,1}.Cross(forward).Normalized() };
	const float cos{ Float3{0,0,1}.Dot(forward) };
	return FromAxis(axis, acos(cos));
}

Math::Quaternion Math::Quaternion::operator*(const Quaternion& other) const
{
	return { {
			m_Real.Cross(other.m_Real) + other.m_Real * m_Complex + m_Real * other.m_Complex
		}, m_Complex * other.m_Complex - m_Real.Dot(other.m_Real) };
}
void Math::Quaternion::operator*=(const Quaternion& other)
{
	m_Real = m_Real.Cross(other.m_Real) + other.m_Real * m_Complex + m_Real * other.m_Complex;
	m_Complex = m_Complex * other.m_Complex - m_Real.Dot(other.m_Real);
}

Math::Quaternion Math::Quaternion::operator-() const
{
	return { -m_Real, m_Complex };
}

void Math::Quaternion::Inverse()
{
	m_Real = -m_Real;
}

Math::Quaternion::Quaternion(const DirectX::XMVECTOR& vector)
{
	XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(&m_Real.x), vector);
}

void Math::Quaternion::operator=(const DirectX::XMVECTOR& vector)
{
	XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(&m_Real.x), vector);
}

void Math::Quaternion::Rotate(const Quaternion& rotation)
{
	*this *= rotation;
	Normalize();
}

void Math::Quaternion::Rotate(const Float3& axis, float radians)
{
	Rotate(FromAxis(axis, radians));
}

void Math::Quaternion::RotatePoint(Float3& point) const
{
	point = (*this * Quaternion{ point, 0 } *-*this).GetReal();
}

Math::Float3 Math::Quaternion::GetRotatedPoint(const Float3& point) const
{
	return (*this * Quaternion{ point, 0 } *-*this).GetReal();
}

Math::Quaternion Math::Quaternion::Normalized() const
{
	const float invLength{ 1.f / GetLength() };
	return { m_Real * invLength, m_Complex * invLength };
}

void Math::Quaternion::Normalize()
{
	const float invLength{ 1.f / GetLength() };
	m_Real *= invLength;
	m_Complex *= invLength;
}

Math::Float3 Math::Quaternion::GetForward() const
{
	return{
		2 * (m_Real.x * m_Real.z + m_Complex * m_Real.y),
		2 * (m_Real.y * m_Real.z - m_Complex * m_Real.x),
		1 - 2 * (m_Real.x * m_Real.x + m_Real.y * m_Real.y) };
}

float Math::Quaternion::GetLength() const
{
	return sqrtf(m_Real.x * m_Real.x + m_Real.y * m_Real.y + m_Real.z * m_Real.z + m_Complex * m_Complex);
}

Math::Float3 Math::Quaternion::GetUp() const
{
	return{
		2 * (m_Real.x * m_Real.y - m_Complex * m_Real.z),
		1 - 2 * (m_Real.x * m_Real.x + m_Real.z * m_Real.z),
		2 * (m_Real.y * m_Real.z + m_Complex * m_Real.x) };
}

Math::Float3 Math::Quaternion::GetRight() const
{
	return{
		(1 - 2 * (m_Real.y * m_Real.y + m_Real.z * m_Real.z)),
		2 * (m_Real.x * m_Real.y + m_Complex * m_Real.z),
		2 * (m_Real.x * m_Real.z - m_Complex * m_Real.y) };
}

