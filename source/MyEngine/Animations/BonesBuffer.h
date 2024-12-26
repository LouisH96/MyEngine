#pragma once
#include <Math\Matrices.h>
#include <Transform\WorldMatrix.h>

namespace MyEngine
{
namespace Animations
{
struct BonesBuffer
{
	static constexpr unsigned MAX_BONES = 100;
	Float4X4 BoneTransforms[MAX_BONES];
	Float4X4 World{ WorldMatrix::GetIdentity() };
};
}
}