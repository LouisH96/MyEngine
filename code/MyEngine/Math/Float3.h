#pragma once

namespace MyEngine
{
	namespace Math
	{
		struct Float3
		{
			Float3();
			Float3(float x, float y, float z);
			Float3(int x, int y, int z);
			explicit Float3(float all);
			explicit Float3(int all);
			~Float3() = default;
			Float3(const Float3& other) = default;
			Float3(Float3&& other) noexcept = default;
			Float3& operator=(const Float3& other) = default;
			Float3& operator=(Float3&& other) noexcept = default;

			Float3 operator+(const Float3& r) const;
			Float3 operator-(const Float3& r) const;
			Float3 operator+(const float& r) const;
			Float3 operator-(const float& r) const;
			Float3 operator*(const float& r) const;
			Float3 operator/(const float& r) const;
			Float3 operator+(const int& r) const;
			Float3 operator-(const int& r) const;
			Float3 operator*(const int& r) const;
			Float3 operator/(const int& r) const;
			void operator+=(const Float3& r);
			void operator-=(const Float3& r);
			void operator+=(const float& r);
			void operator-=(const float& r);
			void operator*=(const float& r);
			void operator/=(const float& r);
			void operator+=(const int& r);
			void operator-=(const int& r);
			void operator*=(const int& r);
			void operator/=(const int& r);
			Float3 operator-() const;

			float Get(int i) const;
			void Set(float f, int i);

			void Reverse();
			void Scale(const Float3& r);

			float x, y, z;
			static constexpr int NR_AXIS = 3;
		};
	}
}
