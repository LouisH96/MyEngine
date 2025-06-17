#pragma once
#include "Vector3.h"

namespace MyEngine
{
template<typename T>
struct Vector4
{
	//---| Fields |---
	T x, y, z, w;

	//---| Constructor/Destructor |---
	Vector4();
	Vector4(T x, T y, T z, T w);
	Vector4(const Vector3<T>& vector3, T w);
	explicit Vector4(T all);

	template<typename D> explicit Vector4(const Vector4<D>& other);

	//---| Rule of five |---
	Vector4(const Vector4& other) = default;
	Vector4(Vector4&& other) noexcept = default;
	Vector4& operator=(const Vector4& other) = default;
	Vector4& operator=(Vector4&& other) noexcept = default;

	//---| Operations |---
	bool operator==(const Vector4& other) const;
	bool operator!=(const Vector4& other)const;
	Vector4 operator+(const Vector4& r) const;
	Vector4 operator-(const Vector4& r) const;
	Vector4 operator+(const T& r) const;
	Vector4 operator-(const T& r) const;
	Vector4 operator*(const T& r) const;
	Vector4 operator/(const T& r) const;
	void operator+=(const Vector4& r);
	void operator-=(const Vector4& r);
	void operator+=(const T& r);
	void operator-=(const T& r);
	void operator*=(const T& r);
	void operator/=(const T& r);
	Vector4 operator-() const;
	T& operator[](int idx);
	T operator[](int idx) const;

	Vector4& operator=(const Vector2<T>& vector2);
	Vector4& operator=(const Vector3<T>& vector3);

	Vector4 operator+(const Vector3<T>& v) const;
	Vector4& operator+=(const Vector3<T>& v);
	Vector4 operator-(const Vector3<T>& v) const;
	Vector4& operator-=(const Vector3<T>& v);

	T Get(int i) const;
	void Set(T f, int i);
	void Inverse();
	void Scale(const Vector4& r);
	T Length() const;
	T LengthSq() const;
	T Dot(const Vector4& other) const;
	T Dot(const Vector3<T>& other) const;
	void Normalize();
	Vector4 Normalized() const;

	//---| Cast |---
	Vector2<T> Xy() const;
	Vector3<T> Xyz() const;
	Vector3<T>& Xyz();
};

template <typename T> Vector4<T>::Vector4() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
template <typename T> Vector4<T>::Vector4(T x, T y, T z, T w) : x{ x }, y{ y }, z{ z }, w{ w } {}
template <typename T> Vector4<T>::Vector4(const Vector3<T>& vector3, T w) : x{ vector3.x }, y{ vector3.y }, z{ vector3.z }, w{ w } {}
template <typename T> Vector4<T>::Vector4(T all) : x{ all }, y{ all }, z{ all }, w{ all } {}

template <typename T>template <typename D>
Vector4<T>::Vector4(const Vector4<D>& other)
	: x{ static_cast<T>(other.x) }, y{ static_cast<T>(other.y) }, z{ static_cast<T>(other.z) }, w{ static_cast<T>(other.w) } {
}

template<typename T>
bool Vector4<T>::operator==(const Vector4<T>& other) const
{
	return x == other.x && y == other.y
		&& z == other.z && w == other.w;
}

template<typename T>
bool Vector4<T>::operator!=(const Vector4<T>& other) const
{
	return !(*this == other);
}

template <typename T> Vector4<T> Vector4<T>::operator+(const Vector4& r) const { return { x + r.x, y + r.y, z + r.z, w + r.w }; }
template <typename T> Vector4<T> Vector4<T>::operator-(const Vector4& r) const { return { x - r.x, y - r.y, z - r.z, w - r.w }; }
template <typename T> Vector4<T> Vector4<T>::operator+(const T& r) const { return { x + r, y + r, z + r, w + r }; }
template <typename T> Vector4<T> Vector4<T>::operator-(const T& r) const { return { x - r, y - r, z - r, w - r }; }
template <typename T> Vector4<T> Vector4<T>::operator*(const T& r) const { return { x * r, y * r, z * r, w * r }; }
template <typename T> Vector4<T> Vector4<T>::operator/(const T& r) const { return { x / r, y / r, z / r, w / r }; }
template <typename T> void Vector4<T>::operator+=(const Vector4& r) { x += r.x; y += r.y; z += r.z; w += r.w; }
template <typename T> void Vector4<T>::operator-=(const Vector4& r) { x -= r.x; y -= r.y; z -= r.z; w -= r.w; }
template <typename T> void Vector4<T>::operator+=(const T& r) { x += r; y += r; z += r; w += r; }
template <typename T> void Vector4<T>::operator-=(const T& r) { x -= r; y -= r; z -= r; w -= r; }
template <typename T> void Vector4<T>::operator*=(const T& r) { x *= r; y *= r; z *= r; w *= r; }
template <typename T> void Vector4<T>::operator/=(const T& r) { x /= r; y /= r; z /= r; w /= r; }
template <typename T> Vector4<T> Vector4<T>::operator-() const { return{ -x, -y, -z, -w }; }

template <typename T>
T& Vector4<T>::operator[](int idx)
{
	return reinterpret_cast<T*>(&x)[idx];
}

template <typename T>
T Vector4<T>::operator[](int idx) const
{
	return reinterpret_cast<const T*>(&x)[idx];
}

template <typename T>
Vector4<T>& Vector4<T>::operator=(const Vector2<T>& vector2)
{
	reinterpret_cast<Vector2<T>&>(x) = vector2;
	return *this;
}

template <typename T>
Vector4<T>& Vector4<T>::operator=(const Vector3<T>& vector3)
{
	reinterpret_cast<Vector3<T>&>(x) = vector3;
	return *this;
}

template<typename T>
inline Vector4<T> Vector4<T>::operator+(const Vector3<T>& v) const
{
	return {
		x + v.x,
		y + v.y,
		z + v.z,
		w
	};
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator+=(const Vector3<T>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
}

template<typename T>
inline Vector4<T> Vector4<T>::operator-(const Vector3<T>& v) const
{
	return {
		x - v.x,
		y - v.y,
		z - v.z,
		w
	};
}

template<typename T>
inline Vector4<T>& Vector4<T>::operator-=(const Vector3<T>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

template <typename T> T Vector4<T>::Get(int i) const { return *(&x + i); }
template <typename T> void Vector4<T>::Set(T f, int i) { *(&x + i) = f; }
template <typename T> void Vector4<T>::Inverse() { x = -x; y = -y; z = -z; w = -w; }
template <typename T> void Vector4<T>::Scale(const Vector4& r) { x *= r.x; y *= r.y; z *= r.z; w *= r.w; }
template <typename T> T Vector4<T>::Length() const { return sqrtf(x * x + y * y + z * z + w * w); }
template <typename T> T Vector4<T>::LengthSq() const { return x * x + y * y + z * z + w * w; }
template <typename T> T Vector4<T>::Dot(const Vector4& other) const { return { x * other.x + y * other.y + z * other.z + w * other.w }; }
template <typename T> T Vector4<T>::Dot(const Vector3<T>& other) const { return { x * other.x + y * other.y + z * other.z }; }

template <typename T>
void Vector4<T>::Normalize()
{
	const T invLength{ static_cast<T>(1) / Length() };
	*this *= invLength;
}

template <typename T>
Vector4<T> Vector4<T>::Normalized() const
{
	const T invLength{ static_cast<T>(1) / Length() };
	return *this * invLength;
}

template<typename T>
Vector2<T> Vector4<T>::Xy() const
{
	return { x, y };
}

template <typename T>
Vector3<T> Vector4<T>::Xyz() const
{
	return{ x,y,z };
}

template <typename T>
Vector3<T>& Vector4<T>::Xyz()
{
	return reinterpret_cast<Vector3<T>&>(x);
}
}
