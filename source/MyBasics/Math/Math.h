#pragma once

namespace MyEngine
{
class Math
{
public:
	template<typename T>
	static T Lerp(float alpha, const T& v0, const T& v1);
};
template<typename T>
inline T Math::Lerp(float alpha, const T& v0, const T& v1)
{
	return (v1 - v0) * alpha + v0;
}
}