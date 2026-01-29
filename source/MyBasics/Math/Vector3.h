#pragma once
#include "Vector2.h"
#include "Scalars.h"

namespace MyEngine
{
template<typename T>
struct Vector3
{
	Vector3();
	Vector3(T x, T y, T z);
	Vector3(const Vector2<T>& other, float z);
	explicit Vector3(T all);
	~Vector3() = default;
	Vector3(const Vector3& other) = default;
	Vector3(Vector3&& other) noexcept = default;
	Vector3& operator=(const Vector3& other) = default;
	Vector3& operator=(Vector3&& other) noexcept = default;

	template<typename O>
	explicit Vector3(const Vector3<O>& other);

	Vector3 operator+(const Vector3& r) const;
	Vector3 operator-(const Vector3& r) const;
	Vector3 operator*(const Vector3& r) const;
	Vector3 operator+(const T& r) const;
	Vector3 operator-(const T& r) const;
	Vector3 operator*(const T& r) const;
	Vector3 operator/(const T& r) const;
	void operator+=(const Vector3& r);
	void operator-=(const Vector3& r);
	void operator*=(const Vector3& r);
	void operator+=(const T& r);
	void operator-=(const T& r);
	void operator*=(const T& r);
	void operator/=(const T& r);
	Vector3 operator-() const;

	bool operator== (const Vector3& r) const;
	bool operator!=(const Vector3& r) const;

	T& operator[](unsigned idx);
	const T& operator[](unsigned idx) const;
	T Get(int i) const;
	void Set(T f, int i);

	Vector2<T> Xy() const;
	Vector2<T> Xz() const;
	void AddXz(const Vector2<T>& xz);
	void SetXz(const Vector2<T>& xz);

	void Reverse(); // -value
	void Scale(const Vector3& r);
	Vector3 Scaled(const Vector3& scale) const;
	void Divide(const Vector3& r);
	Vector3 Divided(const Vector3& r) const;
	T Length() const;
	T LengthSq() const;
	void Normalize();
	Vector3 Normalized() const;
	void Normalize(float& length);
	Vector3 Normalized(float& length) const;
	Vector3 NormalizedSafe() const;
	Vector3 Inversed() const; // 1/value

	Vector3 Cross(const Vector3& other) const;
	float Dot(const Vector3& other) const;

	void LimitLength(T maxLength);
	Vector3 LimitedLength(T maxLength) const;

	float Distance(const Vector3& other) const;
	float DistanceSq(const Vector3& other) const;

	bool HasNan() const;

	//will normalize [0,255] to [0,1]
	static Vector3 Color(const T& r, const T& g, const T& b);
	static Vector3 Color(int hex);
	static Vector3 FromXz(const Vector2<T>& xz);
	static Vector3 FromComponent(unsigned idx, T value, T otherValues);
	static Vector3 TowardZMax(const Vector2<T>& xy, float z = 0);

	T x, y, z;
	static constexpr int NR_AXIS = 3;
};

template <typename T> Vector3<T>::Vector3() : x{ 0 }, y{ 0 }, z{ 0 } {}
template <typename T> Vector3<T>::Vector3(T x, T y, T z) : x{ x }, y{ y }, z{ z } {}

template <typename T> Vector3<T>::Vector3(T all) : x{ all }, y{ all }, z{ all } {}

template <typename T>
template <typename O>
Vector3<T>::Vector3(const Vector3<O>& other)
	: x{ Scalar<T>::Cast(other.x) }
	, y{ Scalar<T>::Cast(other.y) }
	, z{ Scalar<T>::Cast(other.z) }
{
}

template<typename T>
inline Vector3<T>::Vector3(const Vector2<T>& other, float z)
	: x{ other.x }
	, y{ other.y }
	, z{ z }
{
}

template <typename T> Vector3<T> Vector3<T>::operator+(const Vector3& r) const { return { x + r.x, y + r.y, z + r.z }; }
template <typename T> Vector3<T> Vector3<T>::operator-(const Vector3& r) const { return { x - r.x, y - r.y, z - r.z }; }
template <typename T> Vector3<T> Vector3<T>::operator*(const Vector3& r) const { return { x * r.x, y * r.y, z * r.z }; }
template <typename T> Vector3<T> Vector3<T>::operator+(const T& r) const { return { x + r, y + r, z + r }; }
template <typename T> Vector3<T> Vector3<T>::operator-(const T& r) const { return { x - r, y - r, z - r }; }
template <typename T> Vector3<T> Vector3<T>::operator*(const T& r) const { return { x * r, y * r, z * r }; }
template <typename T> Vector3<T> Vector3<T>::operator/(const T& r) const
{
	if constexpr (Scalar<T>::HasDecimals())
	{
		const T inv{ static_cast<T>(1) / r };
		return *this * inv;
	}
	else
	{
		return {
			static_cast<T>(x / r),
			static_cast<T>(y / r),
			static_cast<T>(z / r)
		};
	}
}
template <typename T> void Vector3<T>::operator+=(const Vector3& r) { x += r.x; y += r.y; z += r.z; }
template <typename T> void Vector3<T>::operator-=(const Vector3& r) { x -= r.x; y -= r.y; z -= r.z; }
template <typename T> void Vector3<T>::operator*=(const Vector3& r) { x *= r.x; y *= r.y; z *= r.z; }
template <typename T> void Vector3<T>::operator+=(const T& r) { x += r; y += r; z += r; }
template <typename T> void Vector3<T>::operator-=(const T& r) { x -= r; y -= r; z -= r; }
template <typename T> void Vector3<T>::operator*=(const T& r) { x *= r; y *= r; z *= r; }
template <typename T> void Vector3<T>::operator/=(const T& r)
{
	if constexpr (Scalar<T>::HasDecimals())
	{
		const T inv{ static_cast<T>(1) / r };
		*this *= inv;
	}
	else
	{
		x /= r;
		y /= r;
		z /= r;
	}
}
template <typename T>Vector3<T> Vector3<T>::operator-() const { return { -x, -y, -z }; }

template <typename T>
bool Vector3<T>::operator==(const Vector3& r) const
{
	return x == r.x && y == r.y && z == r.z;
}

template <typename T>
bool Vector3<T>::operator!=(const Vector3& r) const
{
	return x != r.x || y != r.y || z != r.z;
}

template <typename T>
T& Vector3<T>::operator[](unsigned idx)
{
	return *(&x + idx);
}

template <typename T>
const T& Vector3<T>::operator[](unsigned idx) const
{
	return *(&x + idx);
}

template <typename T>
T Vector3<T>::Get(int i) const
{
	if (i == 0) return x;
	if (i == 1) return y;
	return z;
}

template <typename T>
void Vector3<T>::Set(T f, int i)
{
	if (i == 0) x = f;
	else if (i == 1) y = f;
	else z = f;
}

template<typename T>
inline Vector2<T> Vector3<T>::Xy() const
{
	return { x, y };
}

template <typename T>
Vector2<T> Vector3<T>::Xz() const
{
	return { x, z };
}

template<typename T>
inline void Vector3<T>::AddXz(const Vector2<T>& xz)
{
	x += xz.x;
	z += xz.y;
}

template<typename T>
inline void Vector3<T>::SetXz(const Vector2<T>& xz)
{
	x = xz.x;
	z = xz.y;
}

template <typename T>
void Vector3<T>::Reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

template <typename T>
void Vector3<T>::Scale(const Vector3& r)
{
	x *= r.x;
	y *= r.y;
	z *= r.z;
}

template <typename T>
Vector3<T> Vector3<T>::Scaled(const Vector3& scale) const
{
	return { x * scale.x, y * scale.y, z * scale.z };
}

template <typename T>
void Vector3<T>::Divide(const Vector3& r)
{
	x /= r.x;
	y /= r.y;
	z /= r.z;
}

template <typename T>
Vector3<T> Vector3<T>::Divided(const Vector3& r) const
{
	return { x / r.x, y / r.y, z / r.z };
}

template <typename T>
T Vector3<T>::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

template <typename T>
T Vector3<T>::LengthSq() const
{
	return x * x + y * y + z * z;
}

template <typename T>
void Vector3<T>::Normalize()
{
	const float scale = 1.f / Length();
	x *= scale;
	y *= scale;
	z *= scale;
}

template <typename T>
Vector3<T> Vector3<T>::Normalized() const
{
	const float scale = 1.f / Length();
	return { x * scale, y * scale, z * scale };
}

template <typename T>
void Vector3<T>::Normalize(float& length)
{
	length = Length();
	const float scale = 1.f / length;
	x *= scale;
	y *= scale;
	z *= scale;
}

template <typename T>
Vector3<T> Vector3<T>::Normalized(float& length) const
{
	length = Length();
	const float scale = 1.f / length;
	return { x * scale, y * scale, z * scale };
}

template <typename T>
Vector3<T> Vector3<T>::NormalizedSafe() const
{
	const float lengthSq{ LengthSq() };
	if (lengthSq == 0) return *this;
	return *this / sqrtf(lengthSq);
}

template<typename T>
Vector3<T> Vector3<T>::Inversed() const
{
	return { static_cast<T>(1) / x, static_cast<T>(1) / y, static_cast<T>(1) / z };
}

template <typename T>
Vector3<T> Vector3<T>::Cross(const Vector3& other) const
{
	return
	{
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
	};
}

template <typename T>
float Vector3<T>::Dot(const Vector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

template <typename T>
void Vector3<T>::LimitLength(T maxLength)
{
	T value{ LengthSq() };
	if (value <= maxLength * maxLength) return;
	value = maxLength / sqrt(value);
	x *= value;
	y *= value;
	z *= value;
}

template <typename T>
Vector3<T> Vector3<T>::LimitedLength(T maxLength) const
{
	T value{ LengthSq() };
	if (value <= maxLength * maxLength) return *this;
	value = maxLength / sqrt(value);
	return{
		x * value,
		y * value,
		z * value
	};
}

template <typename T>
float Vector3<T>::Distance(const Vector3& other) const
{
	return (other - *this).Length();
}

template <typename T>
float Vector3<T>::DistanceSq(const Vector3& other) const
{
	return (other - *this).LengthSq();
}

template<typename T>
inline bool Vector3<T>::HasNan() const
{
	return isnan(x) || isnan(y) || isnan(z);
}

template <typename T>
Vector3<T> Vector3<T>::Color(const T& r, const T& g, const T& b)
{
	const T scale{ 1 / static_cast<T>(255) };
	return { r * scale, g * scale , b * scale };
}

template <typename T>
Vector3<T> Vector3<T>::Color(int hex)
{
	Vector3 c;
	c.z = static_cast<T>(hex & 0xFF);
	hex >>= 8;
	c.y = static_cast<T>(hex & 0xFF);
	hex >>= 8;
	c.x = static_cast<T>(hex & 0xFF);
	c /= static_cast<T>(255);
	return c;
}

template <typename T>
Vector3<T> Vector3<T>::FromXz(const Vector2<T>& xz)
{
	return { xz.x, 0, xz.y };
}

template <typename T>
Vector3<T> Vector3<T>::FromComponent(unsigned idx, T value, T otherValues)
{
	Vector3 v{ otherValues };
	v[idx] = value;
	return v;
}

template <typename T>
Vector3<T> Vector3<T>::TowardZMax(const Vector2<T>& xy, float z)
{
	return { xy.x, xy.y, z };
}
}
