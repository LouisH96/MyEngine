#include "Camera.h"

#include "../../Logging/Logger.h"

MyEngine::Game::Camera::Camera::Camera(DirectX::XMINT2 size)
{
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
	const float invAspectRatio = static_cast<float>(newSize.y) / static_cast<float>(newSize.x);
	m_Matrix.m[0][0] = invAspectRatio;
}
