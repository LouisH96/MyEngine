#include "pch.h"
#include "Quaternion.h"

Math::Quaternion::Quaternion()
	: Xyz{ 0, 0, 0 }
	, W{ 1 }
{
}

Math::Quaternion::Quaternion(const Float3& real, float complex)
	: Xyz{ real }
	, W{ complex }
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
			Xyz.Cross(other.Xyz) + other.Xyz * W + Xyz * other.W
		}, W * other.W - Xyz.Dot(other.Xyz) };
}
void Math::Quaternion::operator*=(const Quaternion& other)
{
	Xyz = Xyz.Cross(other.Xyz) + other.Xyz * W + Xyz * other.W;
	W = W * other.W - Xyz.Dot(other.Xyz);
}

Math::Quaternion Math::Quaternion::operator-() const
{
	return { -Xyz, W };
}

void Math::Quaternion::Inverse()
{
	Xyz = -Xyz;
}

Math::Quaternion::Quaternion(const DirectX::XMVECTOR& vector)
{
	XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(&Xyz.x), vector);
}

void Math::Quaternion::operator=(const DirectX::XMVECTOR& vector)
{
	XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(&Xyz.x), vector);
}

void Math::Quaternion::RotateBy(const Quaternion& rotation)
{
	*this = rotation * *this;
	Normalize();
}

void Math::Quaternion::RotateBy(const Float3& axis, float radians)
{
	RotateBy(FromAxis(axis, radians));
}

void Math::Quaternion::RotatePoint(Float3& point) const
{
	point = (*this * Quaternion{ point, 0 } *-*this).Xyz;
}

Math::Float3 Math::Quaternion::GetRotatedPoint(const Float3& point) const
{
	return (*this * Quaternion{ point, 0 } *-*this).Xyz;
}

Math::Quaternion Math::Quaternion::Normalized() const
{
	const float invLength{ 1.f / GetLength() };
	return { Xyz * invLength, W * invLength };
}

void Math::Quaternion::Normalize()
{
	const float invLength{ 1.f / GetLength() };
	Xyz *= invLength;
	W *= invLength;
}

Math::Float3 Math::Quaternion::GetForward() const
{
	return{
		2 * (Xyz.x * Xyz.z + W * Xyz.y),
		2 * (Xyz.y * Xyz.z - W * Xyz.x),
		1 - 2 * (Xyz.x * Xyz.x + Xyz.y * Xyz.y) };
}

float Math::Quaternion::GetLength() const
{
	return sqrtf(Xyz.x * Xyz.x + Xyz.y * Xyz.y + Xyz.z * Xyz.z + W * W);
}

Math::Float3 Math::Quaternion::GetUp() const
{
	return{
		2 * (Xyz.x * Xyz.y - W * Xyz.z),
		1 - 2 * (Xyz.x * Xyz.x + Xyz.z * Xyz.z),
		2 * (Xyz.y * Xyz.z + W * Xyz.x) };
}

Math::Float3 Math::Quaternion::GetRight() const
{
	return{
		(1 - 2 * (Xyz.y * Xyz.y + Xyz.z * Xyz.z)),
		2 * (Xyz.x * Xyz.y + W * Xyz.z),
		2 * (Xyz.x * Xyz.z - W * Xyz.y) };
}

