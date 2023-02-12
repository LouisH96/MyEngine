#pragma once

namespace MyEngine
{
	namespace Math
	{
		struct Float2
		{
			Float2();
			Float2(float x, float y);
			Float2(int x, int y);
			explicit Float2(float both);
			explicit Float2(int both);
			~Float2() = default;

			Float2(const Float2& other) = default;
			Float2(Float2 && other) noexcept = default;
			Float2& operator=(const Float2 & other) = default;
			Float2& operator=(Float2 && other) noexcept = default;

			Float2 operator+(const Float2& r) const;
			Float2 operator-(const Float2& r) const;
			Float2 operator+(const float& r) const;
			Float2 operator-(const float& r) const;
			Float2 operator*(const float& r) const;
			Float2 operator/(const float& r) const;
			Float2 operator+(const int& r) const;
			Float2 operator-(const int& r) const;
			Float2 operator*(const int& r) const;
			Float2 operator/(const int& r) const;
			void operator+=(const Float2& r);
			void operator-=(const Float2& r);
			void operator+=(const float& r);
			void operator-=(const float& r);
			void operator*=(const float& r);
			void operator/=(const float& r);
			void operator+=(const int& r);
			void operator-=(const int& r);
			void operator*=(const int& r);
			void operator/=(const int& r);

			float x, y;
		};
	}
}
