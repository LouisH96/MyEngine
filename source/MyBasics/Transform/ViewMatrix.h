#pragma once

#include <Math\Matrices.h>
#include <Math\Vectors.h>

namespace MyEngine
{
class ViewMatrix
{
public:
	static Float4X4 From(const Float3& forward);
	static Float4X4 From(const Float3& forward, const Float3& position);

	static void Scale(Float4X4& matrix, const Float3& scale);
	static void ScaleInv(Float4X4& matrix, const Float3& scale);

	static void SetPosition(Float4X4& viewMatrix, const Float3& position);
};
}

