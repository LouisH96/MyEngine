#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace MyEngine
{
	using Uint2 = Vector2<unsigned>;
	using Uint3 = Vector3<unsigned>;
	using Uint4 = Vector4<unsigned>;

	using Int2 = Vector2<int>;
	using Int3 = Vector3<int>;
	using Int4 = Vector4<int>;

	using Float2 = Vector2<float>;
	using Float3 = Vector3<float>;
	using Float4 = Vector4<float>;

	using Double2 = Vector2<double>;
	using Double3 = Vector3<double>;
	using Double4 = Vector4<double>;
}
