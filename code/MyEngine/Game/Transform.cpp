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
	const XMMATRIX rotation{ XMMatrixRotationQuaternion(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&Rotation.GetReal().x))) };
	const XMMATRIX translation{ XMMatrixTranslationFromVector(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&Position))) };
	return rotation * translation;
}

Math::Float4X4 Game::Transform::GetTransposeInverse() const
{
	const XMMATRIX rotation{ XMMatrixRotationQuaternion(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&Rotation.GetReal().x))) };
	const XMMATRIX translation{ XMMatrixTranslationFromVector(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&Position))) };
	return XMMatrixTranspose(XMMatrixInverse(nullptr, rotation * translation));
}
