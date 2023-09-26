#include "pch.h"
#include "Camera2D.h"

#include "App/ResizedEvent.h"
#include "Rendering/Canvas.h"

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

void Camera2D::OnCanvasResized(const App::ResizedEvent& event)
{
}

void Camera2D::Update()
{
	//todo: rotation
	const float invAspectRatio{ CANVAS.GetInvAspectRatio() };
	m_Buffer.CameraView.Matrix[0].x = invAspectRatio / m_World.Matrix[0].x;
	m_Buffer.CameraView.Matrix[0].z = -m_World.Matrix[0].z * m_Zoom * invAspectRatio;
	m_Buffer.CameraView.Matrix[1].y = 1.f / m_World.Matrix[1].y;
	m_Buffer.CameraView.Matrix[1].z = -m_World.Matrix[1].z * m_Zoom;
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

void Camera2D::SetZoom(float zoom)
{
	const float multiplier{ m_Zoom / zoom };
	m_Zoom = zoom;
	m_World.Matrix[0].x *= multiplier;
	m_World.Matrix[0].y *= multiplier;
	m_World.Matrix[1].x *= multiplier;
	m_World.Matrix[1].y *= multiplier;
}

void Camera2D::Move(const Float2& movement)
{
	m_World.Matrix[0].z += movement.x;
	m_World.Matrix[1].z += movement.y;
}

void Camera2D::MoveScaled(const Float2& movement)
{
	m_World.Matrix[0].z += movement.x / m_Zoom;
	m_World.Matrix[1].z += movement.y / m_Zoom;
}

Float2 Camera2D::GetWorldPos() const
{
	return { m_World.Matrix[0].z, m_World.Matrix[1].z };
}

Float2 Camera2D::GetMouseWorldPos() const
{
	const Float2 mouseNdc{ MOUSE.GetPosNdc() };
	return {
		mouseNdc.x / m_Buffer.CameraView.Matrix[0].x + m_World.Matrix[0].z,
		mouseNdc.y / m_Buffer.CameraView.Matrix[1].y + m_World.Matrix[1].z
	};
}
