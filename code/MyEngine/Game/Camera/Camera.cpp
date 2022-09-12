#include "Camera.h"

#include "../../Logging/Logger.h"
void MyEngine::Game::Camera::Camera::Translate(DirectX::XMFLOAT3 translation)
{
	using namespace DirectX;

	const XMVECTOR xm_current{ XMLoadFloat3(&m_Position) };
	const XMVECTOR xm_translation{ XMLoadFloat3(&translation) };
	XMStoreFloat3(&m_Position, xm_current + xm_translation);

	Logging::Logger::Print(m_Position);
}
