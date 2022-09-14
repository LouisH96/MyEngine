#include "Camera.h"

#include "../../Logging/Logger.h"

void MyEngine::Game::Camera::Camera::Move(DirectX::XMFLOAT3 movement)
{
	using namespace DirectX;

	m_Matrix.m[0][3] -= movement.x;
	m_Matrix.m[1][3] -= movement.y;
	m_Matrix.m[2][3] -= movement.z;
}
