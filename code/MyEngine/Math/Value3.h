#pragma once

namespace MyEngine
{
	namespace Math
	{
		template<typename T>
		struct Value3
		{
			Value3();
			Value3(T x, T y, T z);
			Value3(const DirectX::XMVECTOR& vector);
			explicit Value3(T all);
			~Value3() = default;
			Value3(const Value3& other) = default;
			Value3(Value3&& other) noexcept = default;
			Value3& operator=(const Value3& other) = default;
			Value3& operator=(Value3&& other) noexcept = default;
			Value3& operator=(const DirectX::XMVECTOR& vector);

			Value3 operator+(const Value3& r) const;
			Value3 operator-(const Value3& r) const;
			Value3 operator+(const T& r) const;
			Value3 operator-(const T& r) const;
			Value3 operator*(const T& r) const;
			Value3 operator/(const T& r) const;
			void operator+=(const Value3& r);
			void operator-=(const Value3& r);
			void operator+=(const T& r);
			void operator-=(const T& r);
			void operator*=(const T& r);
			void operator/=(const T& r);
			Value3 operator-() const;

			T Get(int i) const;
			void Set(T f, int i);

			void Reverse();
			void Scale(const Value3& r);
			T Length() const;
			void Normalize();
			Value3 Normalized() const;

			Value3 Cross(const Value3& other) const;
			float Dot(const Value3& other) const;

			T x, y, z;
			static constexpr int NR_AXIS = 3;
		};

		template <typename T> Value3<T>::Value3() : x{ 0 }, y{ 0 }, z{ 0 } {}
		template <typename T> Value3<T>::Value3(T x, T y, T z) : x{ x }, y{ y }, z{ z } {}

		template <typename T>
		Value3<T>::Value3(const DirectX::XMVECTOR& vector)
		{
			XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&x), vector);
		}

		template <typename T>
		Value3<T>& Value3<T>::operator=(const DirectX::XMVECTOR& vector)
		{
			XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&x), vector);
			return *this;
		}

		template <typename T> Value3<T>::Value3(T all) : x{ all }, y{ all }, z{ all } {}
		template <typename T> Value3<T> Value3<T>::operator+(const Value3& r) const { return { x + r.x, y + r.y, z + r.z }; }
		template <typename T> Value3<T> Value3<T>::operator-(const Value3& r) const { return { x - r.x, y - r.y, z - r.z }; }
		template <typename T> Value3<T> Value3<T>::operator+(const T& r) const { return { x + r, y + r, z + r }; }
		template <typename T> Value3<T> Value3<T>::operator-(const T& r) const { return { x - r, y - r, z - r }; }
		template <typename T> Value3<T> Value3<T>::operator*(const T& r) const { return { x * r, y * r, z * r }; }
		template <typename T>Value3<T> Value3<T>::operator/(const T& r) const { return { x / r, y / r, z / r }; }
		template <typename T> void Value3<T>::operator+=(const Value3& r) { x += r.x; y += r.y; z += r.z; }
		template <typename T> void Value3<T>::operator-=(const Value3& r) { x -= r.x; y -= r.y; z -= r.z; }
		template <typename T> void Value3<T>::operator+=(const T& r) { x += r; y += r; z += r; }
		template <typename T> void Value3<T>::operator-=(const T& r) { x -= r; y -= r; z -= r; }
		template <typename T> void Value3<T>::operator*=(const T& r) { x *= r; y *= r; z *= r; }
		template <typename T>void Value3<T>::operator/=(const T& r) { x /= r; y /= r; z /= r; }
		template <typename T>Value3<T> Value3<T>::operator-() const { return { -x, -y, -z }; }

		template <typename T>
		T Value3<T>::Get(int i) const
		{
			if (i == 0) return x;
			if (i == 1) return y;
			return z;
		}

		template <typename T>
		void Value3<T>::Set(T f, int i)
		{
			if (i == 0) x = f;
			else if (i == 1) y = f;
			else z = f;
		}

		template <typename T>
		void Value3<T>::Reverse()
		{
			x = -x;
			y = -y;
			z = -z;
		}

		template <typename T>
		void Value3<T>::Scale(const Value3& r)
		{
			x *= r.x;
			y *= r.y;
			z *= r.z;
		}

		template <typename T>
		T Value3<T>::Length() const
		{
			return sqrtf(x * x + y * y + z * z);
		}

		template <typename T>
		void Value3<T>::Normalize()
		{
			const float scale = 1.f / Length();
			x *= scale;
			y *= scale;
			z *= scale;
		}

		template <typename T>
		Value3<T> Value3<T>::Normalized() const
		{
			const float scale = 1.f / Length();
			return { x * scale, y * scale, z * scale };
		}

		template <typename T>
		Value3<T> Value3<T>::Cross(const Value3& other) const
		{
			return
			{
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			};
		}

		template <typename T>
		float Value3<T>::Dot(const Value3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}
	}
}