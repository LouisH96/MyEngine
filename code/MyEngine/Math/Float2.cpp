#include "pch.h"
#include "Float2.h"

Math::Float2::Float2() : x{ 0 }, y{ 0 } { }
Math::Float2::Float2(float x, float y) : x{ x }, y{ y } { }
Math::Float2::Float2(int x, int y) : x{ static_cast<float>(x) }, y{ static_cast<float>(y) } { }
Math::Float2::Float2(float both) : x{ both }, y{ both } { }
Math::Float2::Float2(int both) : x{ static_cast<float>(both) }, y{ static_cast<float>(both) } { }

Math::Float2 Math::Float2::operator+(const Float2& r) const { return { x + r.x, y + r.y }; }
Math::Float2 Math::Float2::operator-(const Float2& r) const { return { x - r.x, y - r.y }; }
Math::Float2 Math::Float2::operator+(const float& r) const { return { x + r, y + r }; }
Math::Float2 Math::Float2::operator-(const float& r) const { return { x - r, y - r }; }
Math::Float2 Math::Float2::operator*(const float& r) const { return { x * r, y * r }; }
Math::Float2 Math::Float2::operator/(const float& r) const { return { x / r, y / r }; }
Math::Float2 Math::Float2::operator+(const int& r) const { return { x + static_cast<float>(r), y + static_cast<float>(r) }; }
Math::Float2 Math::Float2::operator-(const int& r) const { return { x - static_cast<float>(r), y - static_cast<float>(r) }; }
Math::Float2 Math::Float2::operator*(const int& r) const { return { x * static_cast<float>(r), y * static_cast<float>(r) }; }
Math::Float2 Math::Float2::operator/(const int& r) const { return { x / static_cast<float>(r), y / static_cast<float>(r) }; }
void Math::Float2::operator+=(const Float2& r) { x += r.x; y += r.y; }
void Math::Float2::operator-=(const Float2& r) { x -= r.x; y -= r.y; }
void Math::Float2::operator+=(const float& r) { x += r; y += r; }
void Math::Float2::operator-=(const float& r) { x -= r; y -= r; }
void Math::Float2::operator*=(const float& r) { x *= r; y *= r; }
void Math::Float2::operator/=(const float& r) { x /= r; y /= r; }
void Math::Float2::operator+=(const int& r) { x += static_cast<float>(r); y += static_cast<float>(r); }
void Math::Float2::operator-=(const int& r) { x -= static_cast<float>(r); y -= static_cast<float>(r); }
void Math::Float2::operator*=(const int& r) { x *= static_cast<float>(r); y *= static_cast<float>(r); }
void Math::Float2::operator/=(const int& r) { x /= static_cast<float>(r); y /= static_cast<float>(r); }
