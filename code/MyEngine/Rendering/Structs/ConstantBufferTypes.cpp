#include "pch.h"
#include "ConstantBufferTypes.h"

#include "Game/Transform.h"
#include "Game/Camera/FocusPointCamera.h"

using namespace DirectX;

Rendering::CB_CamMat::CB_CamMat(const Game::FocusPointCamera& cam)
	: CameraMatrix(cam.GetViewProjMatrix())
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Game::FocusPointCamera& cam)
	: CameraMatrix(cam.GetViewProjMatrix())
	, CameraPos(cam.GetPositionFloat3())
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Game::FocusPointCamera& cam, const Game::Transform& transform)
	: CameraMatrix(cam.GetViewProjMatrix())
	, CameraPos(cam.GetPositionFloat3())
{
	const XMMATRIX camMatrix{ XMLoadFloat4x4(&CameraMatrix) };
	const XMMATRIX translation{ XMMatrixTranslation(transform.Position.x, transform.Position.y, transform.Position.z) };
	const XMFLOAT4 quaternion{
		transform.Rotation.GetReal().x,
		transform.Rotation.GetReal().y,
		transform.Rotation.GetReal().z,
		transform.Rotation.GetComplex()
	};
	const XMMATRIX rotation{ XMMatrixRotationQuaternion(XMLoadFloat4(&quaternion)) };
	XMStoreFloat4x4(&CameraMatrix, rotation * translation * camMatrix);
}

Rendering::CB_ModelBuffer::CB_ModelBuffer()
{
}

Rendering::CB_ModelBuffer::CB_ModelBuffer(const Game::Transform& transform)
{
	const XMMATRIX translation{ XMMatrixTranslation(transform.Position.x, transform.Position.y, transform.Position.z) };
	const XMFLOAT4 quaternion{
		transform.Rotation.GetReal().x,
		transform.Rotation.GetReal().y,
		transform.Rotation.GetReal().z,
		transform.Rotation.GetComplex()
	};
	const XMMATRIX rotation{ XMMatrixRotationQuaternion(XMLoadFloat4(&quaternion)) };
	const XMMATRIX model{ rotation * translation };

	XMStoreFloat4x4(&ModelMatrix, model);
	XMStoreFloat4x4(&InvTransposeModelMatrix, XMMatrixTranspose(XMMatrixInverse(nullptr, model)));
}
