#include "pch.h"
#include "ConstantBufferTypes.h"

#include "Game/Transform.h"

using namespace DirectX;

Rendering::CB_CamMat::CB_CamMat(const DirectX::XMMATRIX& viewProjection)
	: CameraMatrix{ viewProjection }
{
}

Rendering::CB_CamMat::CB_CamMat(const DirectX::XMFLOAT4X4& viewProjection)
	: CameraMatrix{ viewProjection }
{
}

Rendering::CB_CamMat::CB_CamMat(const Math::Float3& position, const DirectX::XMMATRIX& viewProjection)
	: CameraMatrix{viewProjection}
{
}

Rendering::CB_CamMat::CB_CamMat(const Math::Float3& position, const Math::Float4X4& viewProjection)
	: CameraMatrix{ viewProjection }
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Math::Float3& position, const Math::Float4X4& matrix)
	: CameraMatrix{ matrix }
	, CameraPos{ position }
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Math::Float3& position, const DirectX::XMFLOAT4X4& matrix)
	: CameraMatrix{ matrix }
	, CameraPos{ position }
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Math::Float3& position, const DirectX::XMMATRIX& matrix)
	: CameraMatrix{matrix}
	, CameraPos{ position }
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Math::Float3& position, const DirectX::XMMATRIX& viewProjection,
	const Game::Transform& transform)
	: CameraMatrix{ transform.AsMatrix() * viewProjection }
	, CameraPos{ position }
{
}

Rendering::CB_ModelBuffer::CB_ModelBuffer(const Game::Transform& transform)
{
	const XMMATRIX world{ transform.AsMatrix() };
	XMStoreFloat4x4(&ModelMatrix, world);
	XMStoreFloat4x4(&InvTransposeModelMatrix, XMMatrixTranspose(XMMatrixInverse(nullptr, world)));
}

Rendering::CB_ModelBuffer::CB_ModelBuffer(const DirectX::XMMATRIX& world, const DirectX::XMMATRIX& transposeInverse)
{
	XMStoreFloat4x4(&ModelMatrix, world);
	XMStoreFloat4x4(&InvTransposeModelMatrix, transposeInverse);
}
