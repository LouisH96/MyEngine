#include "pch.h"
#include "Float3.h"

Math::Float3::Float3() : x{ 0 }, y{ 0 }, z{ 0 } {}
Math::Float3::Float3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
Math::Float3::Float3(int x, int y, int z) : x{ static_cast<float>(x) }, y{ static_cast<float>(y) }, z{ static_cast<float>(z) } {}
Math::Float3::Float3(float all) : x{ all }, y{ all }, z{ all } {}
Math::Float3::Float3(int all) : x{ static_cast<float>(all) }, y{ static_cast<float>(all) }, z{ static_cast<float>(all) } {}

Math::Float3 Math::Float3::operator+(const Float3& r) const { return { x + r.x, y + r.y, z + r.z }; }
Math::Float3 Math::Float3::operator-(const Float3& r) const { return { x - r.x, y - r.y, z - r.z }; }
Math::Float3 Math::Float3::operator+(const float& r) const { return { x + r, y + r, z + r }; }
Math::Float3 Math::Float3::operator-(const float& r) const { return { x - r, y - r, z - r }; }
Math::Float3 Math::Float3::operator*(const float& r) const { return { x * r, y * r, z * r }; }
Math::Float3 Math::Float3::operator/(const float& r) const { return { x / r, y / r, z / r }; }
Math::Float3 Math::Float3::operator+(const int& r) const { return { x + static_cast<float>(r), y + static_cast<float>(r), z + static_cast<float>(r) }; }
Math::Float3 Math::Float3::operator-(const int& r) const { return { x - static_cast<float>(r), y - static_cast<float>(r), z - static_cast<float>(r) }; }
Math::Float3 Math::Float3::operator*(const int& r) const { return { x * static_cast<float>(r), y * static_cast<float>(r), z * static_cast<float>(r) }; }
Math::Float3 Math::Float3::operator/(const int& r) const { return { x / static_cast<float>(r), y / static_cast<float>(r), z / static_cast<float>(r) }; }
void Math::Float3::operator+=(const Float3& r) { x += r.x; y += r.y; z += r.z; }
void Math::Float3::operator-=(const Float3& r) { x -= r.x; y -= r.y; z -= r.z; }
void Math::Float3::operator+=(const float& r) { x += r; y += r; z += r; }
void Math::Float3::operator-=(const float& r) { x -= r; y -= r; z -= r; }
void Math::Float3::operator*=(const float& r) { x *= r; y *= r; z *= r; }
void Math::Float3::operator/=(const float& r) { x /= r; y /= r; z /= r; }
void Math::Float3::operator+=(const int& r) { x += static_cast<float>(r); y += static_cast<float>(r); z += static_cast<float>(r); }
void Math::Float3::operator-=(const int& r) { x -= static_cast<float>(r); y -= static_cast<float>(r); z -= static_cast<float>(r); }
void Math::Float3::operator*=(const int& r) { x *= static_cast<float>(r); y *= static_cast<float>(r); z *= static_cast<float>(r); }
void Math::Float3::operator/=(const int& r) { x /= static_cast<float>(r); y /= static_cast<float>(r); z /= static_cast<float>(r); }

Math::Float3 Math::Float3::operator-() const
{
	return { -x, -y, -z };
}

float Math::Float3::Get(int i) const
{
	if (i == 0) return x;
	if (i == 1) return y;
	return z;
}

void Math::Float3::Set(float f, int i)
{
	if (i == 0) x = f;
	else if (i == 1) y = f;
	else z = f;
}

void Math::Float3::Reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

void Math::Float3::Scale(const Float3& r)
{
	x *= r.x;
	y *= r.y;
	z *= r.z;
}

float Math::Float3::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

void Math::Float3::Normalize()
{
	const float scale = 1.f / Length();
	x *= scale;
	y *= scale;
	z *= scale;
}

Math::Float3 Math::Float3::Normalized() const
{
	const float scale = 1.f / Length();
	return { x * scale, y * scale, z * scale };
}
