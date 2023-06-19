#include "pch.h"
#include "Transform.h"

using namespace DirectX;

Game::Transform::Transform(const Math::Float3& position, const Math::Quaternion& rotation)
	: Position{ position }
	, Rotation{ rotation }
{
}

Game::Transform::Transform(const DirectX::XMMATRIX& matrix)
{
	*this = { matrix };
}

Game::Transform::Transform(const Double4X4& matrix)
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

Game::Transform& Game::Transform::operator=(const DirectX::XMMATRIX& matrix)
{
	XMVECTOR scale;
	XMVECTOR xmQuaternion;
	XMVECTOR xmTranslation;
	XMMatrixDecompose(&scale, &xmQuaternion, &xmTranslation, matrix);
	Rotation = { xmQuaternion };
	XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&Position.x), xmTranslation);
	return *this;
}

Math::Float4X4 Game::Transform::AsMatrix() const
{
	const XMMATRIX rotation{ XMMatrixRotationQuaternion(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&Rotation.Xyz.x))) };
	const XMMATRIX translation{ XMMatrixTranslationFromVector(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&Position))) };
	return rotation * translation;
}

Math::Float4X4 Game::Transform::GetTransposeInverse() const
{
	const XMMATRIX rotation{ XMMatrixRotationQuaternion(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&Rotation.Xyz.x))) };
	const XMMATRIX translation{ XMMatrixTranslationFromVector(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&Position))) };
	return XMMatrixTranspose(XMMatrixInverse(nullptr, rotation * translation));
}

void Game::Transform::LookAt(const Math::Float3& target)
{
	Rotation = Math::Quaternion::FromForward((target - Position).Normalized());
}

Math::Float3 Game::Transform::WorldToLocal(const Math::Float3& worldPoint) const
{
	Math::Float3 local{ worldPoint - Position };
	(-Rotation).RotatePoint(local);
	return local;
}

Float3 Game::Transform::LocalToWorld(const Float3& localPoint) const
{
	return Rotation.GetRotatedPoint(localPoint) + Position;
}

void Game::Transform::SetRelativeTo(const Game::Transform& parent)
{
	Position = (-parent.Rotation).GetRotatedPoint(Position - parent.Position);;
	Rotation = (-parent.Rotation * Rotation).Normalized();
}

Game::Transform Game::Transform::GetRelativeTo(const Transform& parent) const
{
	return{
		(-parent.Rotation).GetRotatedPoint(Position - parent.Position),
		(-parent.Rotation * Rotation).Normalized()
	};
}

Game::Transform Game::Transform::WorldToLocal(const Transform& world, const Transform& parent)
{
	return{
		   (-parent.Rotation).GetRotatedPoint(world.Position - parent.Position),
		   (-parent.Rotation * world.Rotation).Normalized()
	};
}

Game::Transform Game::Transform::LocalToWorld(const Transform& local, const Transform& parent)
{
	return{
		parent.Rotation.GetRotatedPoint(local.Position) + parent.Position,
		(parent.Rotation * local.Rotation).Normalized()
	};
}
