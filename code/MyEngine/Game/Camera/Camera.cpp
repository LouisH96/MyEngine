#include "pch.h"
#include "Camera.h"

#include "Math/Constants.h"

Game::Camera::Camera(Math::Int2 windowSize, float fov, float near, float far)
	: m_Near{ near }
	, m_Far{far}
{
	SetFieldOfView(fov);
	OnWindowResized(windowSize);
}

void Game::Camera::OnWindowResized(Math::Int2 windowSize)
{
	m_InvAspectRatio = static_cast<float>(windowSize.y) / static_cast<float>(windowSize.x);
	UpdateProjectionMatrix();
}

void Game::Camera::SetFieldOfView(float angle)
{
	m_FovValue = 1.f / (angle / 2 * Math::Constants::TO_RAD);
	UpdateProjectionMatrix();
}

const DirectX::XMMATRIX& Game::Camera::GetXmProjectionMatrix() const
{
	return DirectX::XMLoadFloat4x4(&m_ProjectionMatrix);
}

void Game::Camera::UpdateProjectionMatrix()
{
	const float a = m_Far / (m_Far - m_Near);
	const float b = -(m_Far * m_Near) / (m_Far - m_Near);
	m_ProjectionMatrix =
	{
		m_InvAspectRatio * m_FovValue,0,0,0,
		0,m_FovValue,0,0,
		0,0,a,1,
		0,0,b,0
	};
}
