#include "pch.h"
#include "ConstantBufferTypes.h"

#include "Game/Camera/Camera.h"

Rendering::CB_CamMat::CB_CamMat(const Game::Camera& cam)
	: CameraMatrix(cam.GetViewProjMatrix())
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Game::Camera& cam)
	: CameraMatrix(cam.GetViewProjMatrix())
	, CameraPos(cam.GetPositionFloat3())
{
}
