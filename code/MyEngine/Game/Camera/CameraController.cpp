#include "pch.h"
#include "CameraController.h"

#include "Windows.h"
#include <DirectXMath.h>
#include <iostream>

#include "ICamera.h"
#include "App/Input/Keyboard.h"
#include "Math/Math.h"

MyEngine::Game::Camera::CameraController::CameraController(ICamera& camera, App::Input::Keyboard& keyboard)
	: m_Keyboard(keyboard)
	, m_Camera(camera)
{
}

void MyEngine::Game::Camera::CameraController::Update()
{
	DirectX::XMFLOAT3 translation{ 0,0,0 };
	translation.x = static_cast<float>(m_Keyboard.IsDown('D') - m_Keyboard.IsDown('Q'));
	translation.y = static_cast<float>(m_Keyboard.IsDown('E') - m_Keyboard.IsDown('A'));
	translation.z = static_cast<float>(m_Keyboard.IsDown('Z') - m_Keyboard.IsDown('S'));

	//multiply with speed
	constexpr float maxHorSpeed = 1;
	constexpr float maxVerSpeed = 1;
	const float verSpeed = maxVerSpeed * DELTA_TIME;
	float horSpeed = maxHorSpeed * DELTA_TIME;
	if (translation.x != 0 && translation.z != 0) horSpeed *= Math::DIVSQR2;

	translation.x *= horSpeed;
	translation.z *= horSpeed;
	translation.y *= verSpeed;
	m_Camera.Move(translation);

	//ROTATION
	constexpr float maxPitchSpeed = 40.f; //angle/sec
	constexpr float maxYawSpeed = 40.f; //angle/sec
	const float pitchSpeed = maxPitchSpeed * DELTA_TIME;
	const float yawSpeed = maxYawSpeed * DELTA_TIME;
	const float pitch = static_cast<float>(m_Keyboard.IsDown(VK_UP) - m_Keyboard.IsDown(VK_DOWN)) * pitchSpeed;
	const float yaw = static_cast<float>(m_Keyboard.IsDown(VK_LEFT) - m_Keyboard.IsDown(VK_RIGHT)) * yawSpeed;
	m_Camera.Pitch(pitch);
	m_Camera.Yaw(yaw);
}
