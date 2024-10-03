#pragma once

#include <Math\Vectors.h>
#include <Math\Matrices.h>

namespace MyEngine
{
class ViewMatrix
{
public:
	static Float4X4 From(const Float3& forward);
	static Float4X4 From(const Float3& forward, const Float3& position);

	static void SetPosition(Float4X4& viewMatrix, const Float3& position);
};
}

