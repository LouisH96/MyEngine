#include "pch.h"
#include "FpsCameraController.h"

#include "Camera.h"

Game::FpsCameraController::FpsCameraController(const Camera& camera,
	const App::Wrappers::Win32::Window::Keyboard& keyboard,
	const App::Wrappers::Win32::Mouse& mouse)
	: m_Keyboard{ keyboard }
	, m_Mouse{ mouse }
	, m_Camera{ camera }
	, m_LookAroundSpeed{ 0.25, 0.25 }
	, m_Pitch{ 0 }
	, m_Yaw{ 0 }
{
}

void Game::FpsCameraController::Update()
{
	const Math::Int2& mouseDelta = m_Mouse.GetMovement();
	m_Yaw += static_cast<float>(mouseDelta.x) * -m_LookAroundSpeed.x;
	m_Pitch += static_cast<float>(mouseDelta.y) * m_LookAroundSpeed.y;
	if (m_Pitch > 90)
		m_Pitch = 90;
	else if (m_Pitch < -90)
		m_Pitch = -90;
}

DirectX::XMMATRIX Game::FpsCameraController::GetProjectionMatrix() const
{
	return m_Camera.GetXmProjectionMatrix();
}

DirectX::XMMATRIX Game::FpsCameraController::GetWorldMatrix() const
{
	const float pitchRad = m_Pitch * Math::Constants::TO_RAD;
	const float pitchCos = cosf(pitchRad);
	const float pitchSin = sinf(pitchRad);

	const float yawRad = m_Yaw * Math::Constants::TO_RAD;
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

	return {
		xx,xy,xz,0,
		yx,yy,yz,0,
		zx,zy,zz,0,
		m_Position.x, m_Position.y, m_Position.z, 1
	};
}

DirectX::XMMATRIX Game::FpsCameraController::GetViewMatrix() const
{
	return XMMatrixInverse(nullptr, GetWorldMatrix());
}

DirectX::XMMATRIX Game::FpsCameraController::GetViewProjectionMatrix() const
{
	using namespace DirectX;
	const XMMATRIX view{ XMMatrixInverse(nullptr, GetWorldMatrix()) };
	const XMMATRIX projection{ m_Camera.GetXmProjectionMatrix() };
	return view * projection;
}

Math::Float3 Game::FpsCameraController::GetCameraPosition() const
{
	return m_Position;
}

void Game::FpsCameraController::MoveRelative(const Math::Float3& movement)
{
	const float yawRad = m_Yaw * Math::Constants::TO_RAD;
	const float yawCos = cosf(yawRad);
	const float yawSin = sinf(yawRad);
	constexpr float xy = 0;
	const float xx = yawCos;
	const float xz = yawSin;

	m_Position += Math::Float3{ xx,xy,xz } *movement.x;
	m_Position += Math::Float3{ -xz,xy,xx } *movement.z;
	m_Position.y += movement.y;
}
