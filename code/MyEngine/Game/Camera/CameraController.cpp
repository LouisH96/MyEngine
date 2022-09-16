#include "CameraController.h"

#include "Windows.h"
#include <DirectXMath.h>

#include "ICamera.h"
#include "../../App/Input/IInputReader.h"

MyEngine::Game::Camera::CameraController::CameraController(ICamera& camera, App::Input::IInputReader& input)
	: m_Input(input)
	, m_Camera(camera)
{
}

void MyEngine::Game::Camera::CameraController::Update(float dt)
{
	constexpr float maxSpeed = 1;
	constexpr float yMaxSpeed = 1;

	const float speed = maxSpeed * dt;
	const float ySpeed = yMaxSpeed * dt;
	DirectX::XMFLOAT3 translation{ 0,0,0 };
	int nrDirections = 0;

	//x
	if (m_Input.IsKeyDown('Q'))
	{
		translation.x -= speed;
		nrDirections++;
	}
	else if (m_Input.IsKeyDown('D'))
	{
		translation.x += speed;
		nrDirections++;
	}

	//y
	if(m_Input.IsKeyDown('A'))
	{
		translation.y -= ySpeed;
	}
	else if(m_Input.IsKeyDown('E'))
	{
		translation.y += ySpeed;
	}

	//z
	if (m_Input.IsKeyDown('S'))
	{
		translation.z -= speed;
		nrDirections++;
	}
	else if (m_Input.IsKeyDown('Z'))
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
	const float pitchSpeed = maxPitchSpeed * dt;

	if (m_Input.IsKeyDown(VK_UP))
		m_Camera.Pitch(pitchSpeed);
	else if (m_Input.IsKeyDown(VK_DOWN))
		m_Camera.Pitch(-pitchSpeed);

	constexpr float maxYawSpeed = 40.f; //angle/sec
	const float yawSpeed = maxYawSpeed * dt;

	if (m_Input.IsKeyDown(VK_RIGHT))
		m_Camera.Yaw(-yawSpeed);
	else if (m_Input.IsKeyDown(VK_LEFT))
		m_Camera.Yaw(yawSpeed);
}
