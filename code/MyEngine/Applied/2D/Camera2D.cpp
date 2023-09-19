#include "pch.h"
#include "Camera2D.h"

using namespace Applied;

Camera2D::DxMatrix Camera2D::DxMatrix::Identity()
{
	return DxMatrix{
		Float4{1,0,0,0},
		Float4{0,1,0,0},
		Float4{0,0,1,0}
	};
}

Camera2D::Camera2D()
	: m_World{ DxMatrix::Identity() }
{
}

void Camera2D::Update()
{
	//todo: rotation
	m_Buffer.CameraView.Matrix[0].x = 1.f / m_World.Matrix[0].x;
	m_Buffer.CameraView.Matrix[1].y = 1.f / m_World.Matrix[1].y;
}

void Camera2D::AddZoom(float amount)
{
	const float multiplier{ m_Zoom / (m_Zoom + amount) };
	m_Zoom += amount;
	m_World.Matrix[0].x *= multiplier;
	m_World.Matrix[0].y *= multiplier;
	m_World.Matrix[1].x *= multiplier;
	m_World.Matrix[1].y *= multiplier;
}

void Camera2D::ScaleZoom(float amount)
{
	const float multiplier{ 1.f / amount };
	m_Zoom *= amount;
	m_World.Matrix[0].x *= multiplier;
	m_World.Matrix[0].y *= multiplier;
	m_World.Matrix[1].x *= multiplier;
	m_World.Matrix[1].y *= multiplier;
}
