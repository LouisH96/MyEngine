#include "pch.h"
#include "Transform.h"

using namespace DirectX;

Transform::Transform(const Float3& position, const Quaternion& rotation)
	: Position{ position }
	, Rotation{ rotation }
{
}

Transform::Transform(const Double4X4& matrix)
{
	Position = Float3{
		static_cast<float>(matrix[0].w),
		static_cast<float>(matrix[1].w),
		static_cast<float>(matrix[2].w)
	};
	Rotation = Quaternion{ matrix };
}

Transform::Transform(const Float4X4& matrix)
{
	Position = matrix.GetRow3().Xyz();
	Rotation = Quaternion{ matrix };
}

Float4X4 Transform::AsMatrix() const
{
	const XMMATRIX rotation{ XMMatrixRotationQuaternion(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&Rotation.Xyz.x))) };
	const XMMATRIX translation{ XMMatrixTranslationFromVector(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&Position))) };
	return rotation * translation;
}

Float4X4 Transform::GetTransposeInverse() const
{
	const XMMATRIX rotation{ XMMatrixRotationQuaternion(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&Rotation.Xyz.x))) };
	const XMMATRIX translation{ XMMatrixTranslationFromVector(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&Position))) };
	return XMMatrixTranspose(XMMatrixInverse(nullptr, rotation * translation));
}

void Transform::LookAt(const Float3& target)
{
	Rotation = Quaternion::FromForward((target - Position).Normalized());
}

Float3 Transform::WorldToLocal(const Float3& worldPoint) const
{
	Float3 local{ worldPoint - Position };
	(-Rotation).RotatePoint(local);
	return local;
}

Float3 Transform::LocalToWorld(const Float3& localPoint) const
{
	return Rotation.GetRotatedPoint(localPoint) + Position;
}

void Transform::SetRelativeTo(const Transform& parent)
{
	Position = (-parent.Rotation).GetRotatedPoint(Position - parent.Position);;
	Rotation = (-parent.Rotation * Rotation).Normalized();
}

Transform Transform::GetRelativeTo(const Transform& parent) const
{
	return{
		(-parent.Rotation).GetRotatedPoint(Position - parent.Position),
		(-parent.Rotation * Rotation).Normalized()
	};
}

Transform Transform::WorldToLocal(const Transform& world, const Transform& parent)
{
	return{
		   (-parent.Rotation).GetRotatedPoint(world.Position - parent.Position),
		   (-parent.Rotation * world.Rotation).Normalized()
	};
}

Transform Transform::LocalToWorld(const Transform& local, const Transform& parent)
{
	return{
		parent.Rotation.GetRotatedPoint(local.Position) + parent.Position,
		(parent.Rotation * local.Rotation).Normalized()
	};
}
