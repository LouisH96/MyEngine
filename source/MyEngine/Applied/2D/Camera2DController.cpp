#include "pch.h"
#include "Camera2DController.h"

#include "Camera2D.h"

using namespace Applied;

Camera2DController::Camera2DController()
	: m_pCamera{ &CAMERA_2D }
{
}

void Camera2DController::EarlyUpdate()
{
	if (MOUSE.GetScroll())
	{
		const float scale{ Float::Sign(MOUSE.GetScroll()) * m_ZoomSpeed + 1.f };
		m_pCamera->ScaleZoom(scale);
	}

	if (MOUSE.IsMiddleBtnDown())
	{
		const Float2 movement{ Float2{MOUSE.GetMovement()}*Float2{m_MoveSpeed, -m_MoveSpeed} };
		m_pCamera->MoveScaled(-movement);
	}
}

void Camera2DController::SetCamera(Camera2D* pCamera)
{
	m_pCamera = pCamera;
}

void Camera2DController::SetZoom(float zoom) const
{
	m_pCamera->SetZoom(zoom);
}
