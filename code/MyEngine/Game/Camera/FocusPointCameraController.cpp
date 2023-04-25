#include "pch.h"
#include "FocusPointCameraController.h"

#include <DirectXMath.h>
#include <App/Wrappers/Win32/Mouse.h>

#include "Camera.h"
#include "Windows.h"
#include "Math/Constants.h"
#include "Math/Int2.h"

using namespace App::Wrappers::Win32;
using namespace Math;

Game::FocusPointCameraController::FocusPointCameraController(Camera& camera, const Keyboard& keyboard, const Mouse& mouse)
	: m_Keyboard(keyboard)
	, m_Mouse(mouse)
	, m_Camera(camera)
	, m_ScrollSpeed(-1.f)
	, m_HorizontalSpeed(1.f)
	, m_VerticalSpeed{ 1.f }
	, m_FocusPoint{}
	, m_Pitch{ 0 }
	, m_Yaw{ 0 }
	, m_Distance{ 1 }
{
}

void Game::FocusPointCameraController::Update()
{
	//ROTATION
	if (m_Mouse.IsRightBtnDown())
		MouseRotation();
	else
		KeyboardRotation();

	//TRANSLATION
	const float maxVerSpeed = m_VerticalSpeed * Globals::DeltaTime;
	float horSpeed = m_HorizontalSpeed * Globals::DeltaTime;
	if (maxVerSpeed != 0 || horSpeed != 0)
	{
		Float3 translation;
		translation.x = static_cast<float>(m_Keyboard.IsDown('D') - m_Keyboard.IsDown('Q'));
		translation.y = static_cast<float>(m_Keyboard.IsDown('E') - m_Keyboard.IsDown('A')) * maxVerSpeed;
		translation.z = static_cast<float>(m_Keyboard.IsDown('Z') - m_Keyboard.IsDown('S'));

		//multiply with speed
		if (translation.x != 0 && translation.z != 0) horSpeed *= Constants::DIVSQR2;
		translation.x *= horSpeed;
		translation.z *= horSpeed;
		MoveRelative(translation);
	}

	//SCROLL
	m_Distance += m_Mouse.GetScroll() * m_ScrollSpeed;
}

void Game::FocusPointCameraController::SetFocusPoint(const Float3& position)
{
	m_FocusPoint = position;
}

void Game::FocusPointCameraController::SetPitch(float pitch)
{
	m_Pitch = pitch;
}

void Game::FocusPointCameraController::SetYaw(float yaw)
{
	m_Yaw = yaw;
}

void Game::FocusPointCameraController::SetDistance(float distance)
{
	m_Distance = distance;
}

void Game::FocusPointCameraController::MoveRelative(const Math::Float3& movement)
{
	const float yawRad = m_Yaw * Constants::TO_RAD;
	const float yawCos = cosf(yawRad);
	const float yawSin = sinf(yawRad);
	constexpr float xy = 0;
	const float xx = yawCos;
	const float xz = yawSin;

	m_FocusPoint += Float3{ xx,xy,xz } *movement.x;
	m_FocusPoint += Float3{ -xz,xy,xx } *movement.z;
	m_FocusPoint.y += movement.y;
}

DirectX::XMFLOAT4X4 Game::FocusPointCameraController::GetWorldMatrix() const
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

	return {
		xx,xy,xz,0,
		yx,yy,yz,0,
		zx,zy,zz,0,
		-zx * m_Distance + m_FocusPoint.x,
		-zy * m_Distance + m_FocusPoint.y,
		-zz * m_Distance + m_FocusPoint.z,
		1
	};
}

DirectX::XMMATRIX Game::FocusPointCameraController::GetViewProjectionMatrix() const
{
	using namespace DirectX;
	XMFLOAT4X4 matrix{ GetWorldMatrix() };
	const XMMATRIX view{ XMMatrixInverse(nullptr, XMLoadFloat4x4(&matrix)) };
	matrix = m_Camera.GetProjectionMatrix();
	const XMMATRIX projection{ XMLoadFloat4x4(&matrix) };
	return view * projection;
}

Float3 Game::FocusPointCameraController::GetCameraPosition() const
{
	const float pitchRad = m_Pitch * Constants::TO_RAD;
	const float pitchCos = cosf(pitchRad);
	const float pitchSin = sinf(pitchRad);

	const float yawRad = m_Yaw * Constants::TO_RAD;
	const float yawCos = cosf(yawRad);
	const float yawSin = sinf(yawRad);

	const float zx = -yawSin * pitchCos;
	const float zy = -pitchSin;
	const float zz = yawCos * pitchCos;
	return {
		-zx * m_Distance + m_FocusPoint.x,
		-zy * m_Distance + m_FocusPoint.y,
		-zz * m_Distance + m_FocusPoint.z };
}

void Game::FocusPointCameraController::KeyboardRotation()
{
	constexpr float maxPitchSpeed = 100.f; //degrees/sec
	constexpr float maxYawSpeed = 100.f; //degrees/sec
	const float pitchSpeed = maxPitchSpeed * Globals::DeltaTime;
	const float yawSpeed = maxYawSpeed * Globals::DeltaTime;
	const float pitch = static_cast<float>(m_Keyboard.IsDown(VK_UP) - m_Keyboard.IsDown(VK_DOWN)) * pitchSpeed;
	const float yaw = static_cast<float>(m_Keyboard.IsDown(VK_LEFT) - m_Keyboard.IsDown(VK_RIGHT)) * yawSpeed;
	m_Yaw += yaw;
	m_Pitch += pitch;
}

void Game::FocusPointCameraController::MouseRotation()
{
	constexpr float radiansPerPixel = Constants::PI / 10;
	const Int2& mouseDelta = m_Mouse.GetMovement();
	m_Yaw += static_cast<float>(mouseDelta.x) * -radiansPerPixel;
	m_Pitch += static_cast<float>(mouseDelta.y) * radiansPerPixel;
}
