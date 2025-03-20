#include "pch.h"
#include "PointTriangleCollision.h"

Float3 PointTriangleCollision::Closest(
	const Float3& point, const Float3* pTrianglePoints)
{
	/*
		Simply copied from the RealTime-Collision-Detection book (p.141).
	*/
	const Float3& p{ point };
	const Float3& a{ pTrianglePoints[0] };
	const Float3& b{ pTrianglePoints[1] };
	const Float3& c{ pTrianglePoints[2] };

	const Float3 ab{ b - a };
	const Float3 ac{ c - a };
	const Float3 ap{ p - a };

	const float d1{ ab.Dot(ap) };
	const float d2{ ac.Dot(ap) };
	if (d1 <= 0 && d2 <= 0)
		return a;

	const Float3 bp{ p - b };
	const float d3{ ab.Dot(bp) };
	const float d4{ ac.Dot(bp) };
	if (d3 >= 0 && d4 <= d3)
		return b;

	const float vc{ d1 * d4 - d3 * d2 };
	if (vc <= 0 && d1 >= 0 && d3 <= 0)
	{
		const float v{ d1 / (d1 - d3) };
		return a + ab * v;
	}

	const Float3 cp{ p - c };
	const float d5{ ab.Dot(cp) };
	const float d6{ ac.Dot(cp) };
	if (d6 >= 0 && d5 <= d6)
		return c;

	const float vb{ d5 * d2 - d1 * d6 };
	if (vb <= 0 && d2 >= 0 && d6 <= 0)
	{
		const float w{ d2 / (d2 - d6) };
		return a + ac * w;
	}

	const float va{ d3 * d6 - d5 * d4 };
	if(va <= 0 && (d4-d3) >= 0 && (d5-d6)>=0)
	{
		const float w{ (d4 - d3) / ((d4 - d3) + (d5 - d6)) };
		return b + (c - b) * w;
	}

	const float denom{ 1.f / (va + vb + vc) };
	const float v{ vb * denom };
	const float w{ vc * denom };
	return a + ab * v + ac * w;
}
