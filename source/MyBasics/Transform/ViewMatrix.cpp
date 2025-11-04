#include "ViewMatrix.h"

using namespace MyEngine;

Float4X4 ViewMatrix::From(const Float3& forward)
{
	Float3 right;
	if (forward.x == 0 && abs(forward.y) == 1 && forward.z == 0)
		right = { 1,0,0 };
	else
		right = Float3{ 0,1,0 }.Cross(forward).Normalized();
	const Float3 up{ forward.Cross(right).Normalized() };

	return Float4X4{
		Float4{right, 0},
		Float4{up, 0},
		Float4{forward, 0},
		Float4{0,0,0,1}
	};
}

Float4X4 ViewMatrix::From(const Float3& forward, const Float3& position)
{
	Float3 right;
	if (forward.x == 0 && abs(forward.y) == 1 && forward.z == 0)
		right = { 1,0,0 };
	else
		right = Float3{ 0,1,0 }.Cross(forward).Normalized();
	const Float3 up{ forward.Cross(right).Normalized() };

	return Float4X4{
		Float4{right, -right.Dot(position)},
		Float4{up, -up.Dot(position)},
		Float4{forward, -forward.Dot(position)},
		Float4{0,0,0,1}
	};
}

void ViewMatrix::Scale(Float4X4& matrix, const Float3& scale)
{
	for (unsigned iCol{ 0 }; iCol < 3; ++iCol)
		matrix[iCol] /= scale[iCol];
}

void ViewMatrix::ScaleInv(Float4X4& matrix, const Float3& scale)
{
	for (unsigned iCol{ 0 }; iCol < 3; ++iCol)
		matrix[iCol] *= scale[iCol];
}

void ViewMatrix::SetPosition(Float4X4& viewMatrix, const Float3& position)
{
	viewMatrix[0][3] = -viewMatrix.GetCol0().Dot(position);
	viewMatrix[1][3] = -viewMatrix.GetCol1().Dot(position);
	viewMatrix[2][3] = -viewMatrix.GetCol2().Dot(position);
}
