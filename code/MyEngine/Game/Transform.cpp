#include "pch.h"
#include "Transform.h"

DirectX::XMMATRIX Game::Transform::AsMatrix() const
{
	using namespace DirectX;
	const XMMATRIX rotation{ XMMatrixRotationQuaternion(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&Rotation.GetReal().x))) };
	const XMMATRIX translation{ XMMatrixTranslationFromVector(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&Position))) };
	return translation * rotation;
}
