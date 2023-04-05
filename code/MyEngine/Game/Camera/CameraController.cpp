#include "pch.h"
#include "CameraController.h"

#include <DirectXMath.h>
#include <App/Wrappers/Win32/Mouse.h>
#include "Camera.h"
#include "Windows.h"
#include "Math/Constants.h"
#include "Math/Int2.h"

using namespace App::Wrappers::Win32;
using namespace Math;

Game::CameraController::CameraController(Camera& camera, const Keyboard& keyboard, const Mouse& mouse)
	: m_Keyboard(keyboard)
	, m_Mouse(mouse)
	, m_Camera(camera)
	, m_ScrollSpeed(-1.f)
	, m_HorizontalSpeed(1.f)
{
}

void Game::CameraController::Update()
{
	//TRANSLATION
	DirectX::XMFLOAT3 translation{};
	translation.x = static_cast<float>(m_Keyboard.IsDown('D') - m_Keyboard.IsDown('Q'));
	translation.y = static_cast<float>(m_Keyboard.IsDown('E') - m_Keyboard.IsDown('A'));
	translation.z = static_cast<float>(m_Keyboard.IsDown('Z') - m_Keyboard.IsDown('S'));

	//multiply with speed
	constexpr float maxVerSpeed = 1;
	float horSpeed = m_HorizontalSpeed * DELTA_TIME;
	if (translation.x != 0 && translation.z != 0) horSpeed *= Constants::DIVSQR2;

	translation.x *= horSpeed;
	translation.z *= horSpeed;
	translation.y *= maxVerSpeed * DELTA_TIME;
	m_Camera.Move(translation);

	//ROTATION
	if (m_Mouse.IsRightBtnDown())
		MouseRotation();
	else
		KeyboardRotation();

	//SCROLL
	m_Camera.Zoom(m_Mouse.GetScroll() * m_ScrollSpeed);
}

void Game::CameraController::KeyboardRotation() const
{
	constexpr float maxPitchSpeed = 100.f; //degrees/sec
	constexpr float maxYawSpeed = 100.f; //degrees/sec
	const float pitchSpeed = maxPitchSpeed * DELTA_TIME;
	const float yawSpeed = maxYawSpeed * DELTA_TIME;
	const float pitch = static_cast<float>(m_Keyboard.IsDown(VK_UP) - m_Keyboard.IsDown(VK_DOWN)) * pitchSpeed;
	const float yaw = static_cast<float>(m_Keyboard.IsDown(VK_LEFT) - m_Keyboard.IsDown(VK_RIGHT)) * yawSpeed;
	m_Camera.Pitch(pitch);
	m_Camera.Yaw(yaw);
}

void Game::CameraController::MouseRotation() const
{
	constexpr float radiansPerPixel = Constants::PI / 10;
	const Int2& mouseDelta = m_Mouse.GetMovement();
	m_Camera.Yaw(static_cast<float>(mouseDelta.x) * -radiansPerPixel);
	m_Camera.Pitch(static_cast<float>(mouseDelta.y) * radiansPerPixel);
}
