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
}

void Camera2DController::SetCamera(Camera2D* pCamera)
{
	m_pCamera = pCamera;
}
