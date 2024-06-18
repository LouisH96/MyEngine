#include "Vectors.h"

using namespace MyEngine;

Float3 MyEngine::operator*(const Uint3& l, const Float3& r)
{
	return {
		l.x * r.x,
		l.y * r.y,
		l.z * r.z
	};
}

Float3 MyEngine::operator*(const Float3& l, const Uint3& r)
{
	return {
		l.x * r.x,
		l.y * r.y,
		l.z * r.z
	};
}
