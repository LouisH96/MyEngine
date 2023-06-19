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

Quaternion::Quaternion(const Double4X4& matrix)
{
	https://d3cw3dd2w32x2b.cloudfront.net/wp-content/uploads/2015/01/matrix-to-quat.pdf
	double t;
	if (matrix[2][2] < 0)
	{
		if (matrix[0][0] > matrix[1][1])
		{
			t = 1 + matrix[0][0] - matrix[1][1] - matrix[2][2];
			Xyz = Float3{ static_cast<float>(t), static_cast<float>(matrix[1][0] + matrix[0][1]), static_cast<float>(matrix[0][2] + matrix[2][0]) };
			W = matrix[2][1] - matrix[1][2];
		}
		else
		{
			t = 1 - matrix[0][0] + matrix[1][1] - matrix[2][2];
			Xyz = Float3{ static_cast<float>(matrix[1][0] + matrix[0][1]), static_cast<float>(t), static_cast<float>(matrix[2][1] + matrix[1][2]) };
			W = matrix[0][2] - matrix[2][0];
		}
	}
	else
	{
		if (matrix[0][0] < -matrix[1][1])
		{
			t = 1 - matrix[0][0] - matrix[1][1] + matrix[2][2];
			Xyz = Float3{ static_cast<float>(matrix[0][2] + matrix[2][0]), static_cast<float>(matrix[2][1] + matrix[1][2]), static_cast<float>(t) };
			W = matrix[1][0] - matrix[0][1];
		}
		else
		{
			t = 1 + matrix[0][0] + matrix[1][1] + matrix[2][2];
			Xyz = Float3{ static_cast<float>(matrix[2][1] - matrix[1][2]), static_cast<float>(matrix[0][2] - matrix[2][0]), static_cast<float>(matrix[1][0] - matrix[0][1]) };
			W = static_cast<float>(t);
		}
	}
	const float s{ static_cast<float>(0.5 / sqrt(t)) };
	Xyz *= s;
	W *= s;
}

Quaternion::Quaternion(const Float4X4& matrix)
{
	float t;
	if (matrix[2][2] < 0)
	{
		if (matrix[0][0] > matrix[1][1])
		{
			t = 1 + matrix[0][0] - matrix[1][1] - matrix[2][2];
			Xyz = Float3{ t, (matrix[1][0] + matrix[0][1]), (matrix[0][2] + matrix[2][0]) };
			W = matrix[2][1] - matrix[1][2];
		}
		else
		{
			t = 1 - matrix[0][0] + matrix[1][1] - matrix[2][2];
			Xyz = Float3{ (matrix[1][0] + matrix[0][1]), t, (matrix[2][1] + matrix[1][2]) };
			W = matrix[0][2] - matrix[2][0];
		}
	}
	else
	{
		if (matrix[0][0] < -matrix[1][1])
		{
			t = 1 - matrix[0][0] - matrix[1][1] + matrix[2][2];
			Xyz = Float3{ (matrix[0][2] + matrix[2][0]), (matrix[2][1] + matrix[1][2]), t };
			W = matrix[1][0] - matrix[0][1];
		}
		else
		{
			t = 1 + matrix[0][0] + matrix[1][1] + matrix[2][2];
			Xyz = Float3{ (matrix[2][1] - matrix[1][2]), (matrix[0][2] - matrix[2][0]), (matrix[1][0] - matrix[0][1]) };
			W = t;
		}
	}
	const float s{ (0.5f / sqrt(t)) };
	Xyz *= s;
	W *= s;
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

Quaternion Quaternion::FromEulerDegrees(const Float3& eulers)
{
	//https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
	constexpr float multiplier{ Constants::TO_RAD * 0.5 };
	const float cr = cos(eulers.x * multiplier);
	const float sr = sin(eulers.x * multiplier);
	const float cp = cos(eulers.y * multiplier);
	const float sp = sin(eulers.y * multiplier);
	const float cy = cos(eulers.z * multiplier);
	const float sy = sin(eulers.z * multiplier);

	return{ Float3{
		sr * cp * cy - cr * sp * sy,
		cr * sp * cy + sr * cp * sy,
		cr * cp * sy - sr * sp * cy},
		cr* cp* cy + sr * sp * sy };
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

