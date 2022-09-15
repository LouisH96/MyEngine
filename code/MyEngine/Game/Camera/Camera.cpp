#include "Camera.h"

#include "../../Logging/Logger.h"
#include <cmath>

MyEngine::Game::Camera::Camera::Camera(DirectX::XMINT2 size)
{
	SetFieldOfView(90.f);
	OnWindowResized(size);
}

void MyEngine::Game::Camera::Camera::Move(DirectX::XMFLOAT3 movement)
{
	using namespace DirectX;

	m_Matrix.m[3][0] -= movement.x;
	m_Matrix.m[3][1] -= movement.y;
	m_Matrix.m[3][2] -= movement.z;

	m_Matrix.m[3][3] -= movement.z;
}

void MyEngine::Game::Camera::Camera::OnWindowResized(DirectX::XMINT2 newSize)
{
	m_InvAspectRatio = static_cast<float>(newSize.y) / static_cast<float>(newSize.x);

	m_Matrix.m[0][0] = m_InvAspectRatio * m_FovValue;
}

void MyEngine::Game::Camera::Camera::SetFieldOfView(float angle)
{
	using namespace DirectX;
	m_FovValue = 1.f / tanf(XMConvertToRadians(angle / 2.f));

	m_Matrix.m[0][0] = m_FovValue * m_InvAspectRatio;
	m_Matrix.m[1][1] = m_FovValue;
}
