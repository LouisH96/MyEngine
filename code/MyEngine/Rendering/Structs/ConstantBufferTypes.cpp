#include "pch.h"
#include "ConstantBufferTypes.h"

#include "Game/Transform.h"
#include "Game/Camera/Camera.h"

using namespace DirectX;

Rendering::CB_CamMat::CB_CamMat(const Game::Camera& cam)
	: CameraMatrix(cam.GetViewProjMatrix())
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Game::Camera& cam)
	: CameraMatrix(cam.GetViewProjMatrix())
	, CameraPos(cam.GetPositionFloat3())
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Game::Camera& cam, const Game::Transform& transform)
	: CameraMatrix(cam.GetViewProjMatrix())
	, CameraPos(cam.GetPositionFloat3())
{

	const XMMATRIX camMatrix{ XMLoadFloat4x4(&CameraMatrix) };
	const XMMATRIX translation{ XMMatrixTranslation(transform.Position.x, transform.Position.y, transform.Position.z) };
	XMStoreFloat4x4(&CameraMatrix, XMMatrixMultiply(translation, camMatrix));
}

Rendering::CB_ModelBuffer::CB_ModelBuffer()
{
}

Rendering::CB_ModelBuffer::CB_ModelBuffer(const Game::Transform& transform)
{
	XMStoreFloat4x4(&ModelMatrix, XMMatrixTranslation(transform.Position.x, transform.Position.y, transform.Position.z));
}
