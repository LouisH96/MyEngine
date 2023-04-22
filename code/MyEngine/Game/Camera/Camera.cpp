#include "pch.h"
#include "Camera.h"

#include <cmath>
#include <Math/Constants.h>

using namespace Math;

Game::Camera::Camera(DirectX::XMINT2 size)
{
	SetFieldOfView(90.f);
	OnWindowResized(size);
}

void Game::Camera::Update()
{
	UpdateWorldMatrix();
	UpdateViewProjMatrix();
}

void Game::Camera::Move(DirectX::XMFLOAT3 movement)
{
	using namespace DirectX;

	const float rad = m_Yaw * Constants::TO_RAD;
	const float cos = cosf(rad);
	const float sin = sinf(rad);

	m_FocusPoint.x += movement.x * cos - movement.z * sin;
	m_FocusPoint.z += movement.x * sin + movement.z * cos;
	m_FocusPoint.y += movement.y;
}

void Game::Camera::MoveInDirection(DirectX::XMFLOAT3 movement)
{
	using namespace DirectX;
	XMVECTOR xm_pos{ XMLoadFloat3(&m_FocusPoint) };

	XMVECTOR xm_axis{ XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(m_World.m[0])) };
	xm_pos += xm_axis * movement.x;

	xm_axis = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(m_World.m[1]));
	xm_pos += xm_axis * movement.y;

	xm_axis = XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(m_World.m[2]));
	xm_pos += xm_axis * movement.z;

	XMStoreFloat3(&m_FocusPoint, xm_pos);
}

void Game::Camera::SetFocusPoint(const Math::Float3& focus)
{
	m_FocusPoint.x = focus.x;
	m_FocusPoint.y = focus.y;
	m_FocusPoint.z = focus.z;
}

void Game::Camera::Pitch(float addAngle)
{
	m_Pitch += addAngle;
}

void Game::Camera::Yaw(float addAngle)
{
	m_Yaw += addAngle;
}

void Game::Camera::Zoom(float addDistance)
{
	m_FocusPointDistance += addDistance;
}

void Game::Camera::OnWindowResized(DirectX::XMINT2 newSize)
{
	m_InvAspectRatio = static_cast<float>(newSize.y) / static_cast<float>(newSize.x);
	UpdateProjMatrix();
}

void Game::Camera::SetFieldOfView(float angle)
{
	using namespace DirectX;
	m_FovValue = 1.f / tanf(XMConvertToRadians(angle / 2.f));
	UpdateProjMatrix();
}

DirectX::XMFLOAT3 Game::Camera::GetPosition() const
{
	return { m_World(3,0), m_World(3,1), m_World(3,2) };
}

Float3 Game::Camera::GetPositionFloat3() const
{
	return { m_World(3,0), m_World(3,1), m_World(3,2) };
}

void Game::Camera::UpdateWorldMatrix()
{
	const float pitchRad = m_Pitch * Constants::TO_RAD;
	const float pitchCos = cosf(pitchRad);
	const float pitchSin = sinf(pitchRad);

	const float yawRad = m_Yaw * Constants::TO_RAD;
	const float yawCos = cosf(yawRad);
	const float yawSin = sinf(yawRad);

	constexpr float xy = 0;
	const float xx = yawCos;
	const float xz = yawSin;

	const float yx = -yawSin * pitchSin;
	const float yy = pitchCos;
	const float yz = yawCos * pitchSin;

	const float zx = -yawSin * pitchCos;
	const float zy = -pitchSin;
	const float zz = yawCos * pitchCos;

	m_World =
	{
		xx,xy,xz,0,
		yx,yy,yz,0,
		zx,zy,zz,0,
		-zx * m_FocusPointDistance + m_FocusPoint.x,
		-zy * m_FocusPointDistance + m_FocusPoint.y,
		-zz * m_FocusPointDistance + m_FocusPoint.z,
		1
	};
}

void Game::Camera::UpdateProjMatrix()
{
	constexpr float far = 50;
	constexpr float near = .01f;

	constexpr float a = far / (far - near);
	constexpr float b = -(far * near) / (far - near);

	m_Proj =
	{
		m_InvAspectRatio * m_FovValue,0,0,0,
		0,m_FovValue,0,0,
		0,0,a,1,
		0,0,b,0
	};
}

void Game::Camera::UpdateViewProjMatrix()
{
	using namespace DirectX;
	const XMMATRIX xm_world = XMLoadFloat4x4(&m_World);
	const XMMATRIX xm_proj = XMLoadFloat4x4(&m_Proj);
	const XMMATRIX xm_viewProj = XMMatrixMultiply(XMMatrixInverse(nullptr, xm_world), xm_proj);
	XMStoreFloat4x4(&m_ViewProjMatrix, xm_viewProj);
}
