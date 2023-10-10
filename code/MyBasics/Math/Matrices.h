#pragma once
#include "DxMatrix3X3.h"
#include "Matrix2X2.h"
#include "Matrix3X3.h"
#include "Matrix4x4.h"

namespace MyEngine
{
	using Float2X2 = Matrix2X2<float>;

	using Float3X3 = Matrix3X3<float>;

	using DxFloat3X3 = DxMatrix3X3<float>;

	using Int4X4 = Matrix4X4<int>;
	using Float4X4 = Matrix4X4<float>;
	using Double4X4 = Matrix4X4<double>;
}
