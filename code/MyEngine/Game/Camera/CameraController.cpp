#include "pch.h"
#include "CameraController.h"

#include "Windows.h"
#include <DirectXMath.h>
#include <iostream>

#include "ICamera.h"
#include "App/Input/Keyboard.h"

MyEngine::Game::Camera::CameraController::CameraController(ICamera& camera, App::Input::Keyboard& keyboard)
	: m_Keyboard(keyboard)
	, m_Camera(camera)
{
}

void MyEngine::Game::Camera::CameraController::Update()
{
	constexpr float maxSpeed = 1;
	constexpr float yMaxSpeed = 1;

	const float speed = maxSpeed * DELTA_TIME;
	const float ySpeed = yMaxSpeed * DELTA_TIME;
	DirectX::XMFLOAT3 translation{ 0,0,0 };
	int nrDirections = 0;

	//x
	if (m_Keyboard.IsDown('Q'))
	{
		translation.x -= speed;
		nrDirections++;
	}
	else if (m_Keyboard.IsDown('D'))
	{
		translation.x += speed;
		nrDirections++;

		std::cout << "d down\n";
	}

	//y
	if(m_Keyboard.IsDown('A'))
	{
		translation.y -= ySpeed;
	}
	else if(m_Keyboard.IsDown('E'))
	{
		translation.y += ySpeed;
		std::cout << "e down\n";
		bool r = m_Keyboard.IsDown('E');
	}

	//z
	if (m_Keyboard.IsDown('S'))
	{
		translation.z -= speed;
		nrDirections++;
	}
	else if (m_Keyboard.IsDown('Z'))
	{
		translation.z += speed;
		nrDirections++;
	}
	
	if(nrDirections > 1)
	{
		translation.x /= SQR2;
		translation.z /= SQR2;
	}

	m_Camera.Move(translation);

	//ROTATION
	constexpr float maxPitchSpeed = 40.f; //angle/sec
	const float pitchSpeed = maxPitchSpeed * DELTA_TIME;

	if (m_Keyboard.IsDown(VK_UP))
		m_Camera.Pitch(pitchSpeed);
	else if (m_Keyboard.IsDown(VK_DOWN))
		m_Camera.Pitch(-pitchSpeed);

	constexpr float maxYawSpeed = 40.f; //angle/sec
	const float yawSpeed = maxYawSpeed * DELTA_TIME;

	if (m_Keyboard.IsDown(VK_RIGHT))
		m_Camera.Yaw(-yawSpeed);
	else if (m_Keyboard.IsDown(VK_LEFT))
		m_Camera.Yaw(yawSpeed);
}
