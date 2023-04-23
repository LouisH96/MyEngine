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

DirectX::XMFLOAT4X4 Game::Camera::GetViewProjectionMatrix() const
{
	using namespace DirectX;
	const XMMATRIX rotation{ XMMatrixRotationQuaternion(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&m_Transform.Rotation.GetReal().x))) };
	const XMMATRIX translation{ XMMatrixTranslationFromVector(XMLoadFloat3(reinterpret_cast<const XMFLOAT3*>(&m_Transform.Position))) };
	const XMMATRIX world{ translation * rotation };
	const XMMATRIX projection{ XMLoadFloat4x4(&m_ProjectionMatrix) };
	const XMMATRIX view{ XMMatrixInverse(nullptr, world) };
	const XMMATRIX viewProjection{ view * projection };
	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result, viewProjection);
	return result;
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

void Game::Camera::SetPosition(const Math::Float3& position)
{
	m_Transform.Position = position;
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
