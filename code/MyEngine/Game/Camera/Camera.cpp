#include "Camera.h"

#include "../../Logging/Logger.h"

void MyEngine::Game::Camera::Camera::Move(DirectX::XMFLOAT3 movement)
{
	using namespace DirectX;

	m_Matrix.m[3][0] -= movement.x;
	m_Matrix.m[3][1] -= movement.y;
	m_Matrix.m[3][2] -= movement.z;

	m_Matrix.m[3][3] -= movement.z;
}
