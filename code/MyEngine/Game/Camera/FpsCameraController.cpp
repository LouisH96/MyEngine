#include "pch.h"
#include "FpsCameraController.h"

#include "Camera.h"

FpsCameraController::FpsCameraController()
	: FpsCameraController{ *Globals::pCamera }
{
}

FpsCameraController::FpsCameraController(Camera& camera)
	: m_Camera{ camera }
	, m_LookAroundSpeed{ 0.25, 0.25 }
	, m_Pitch{ 0 }
	, m_Yaw{ 0 }
{
}

void FpsCameraController::Update()
{
	const Int2& mouseDelta = Globals::pMouse->GetMovement();
	m_Yaw += static_cast<float>(mouseDelta.x) * -m_LookAroundSpeed.x;
	m_Pitch += static_cast<float>(mouseDelta.y) * m_LookAroundSpeed.y;
	if (m_Pitch > 90)
		m_Pitch = 90;
	else if (m_Pitch < -90)
		m_Pitch = -90;

	//CAMERA
	m_Camera.SetRotation(m_Pitch * Constants::TO_RAD, m_Yaw * Constants::TO_RAD);
	m_Camera.SetPosition(m_Position);
}

void FpsCameraController::SetPositionXz(const Float2& position)
{
	m_Position.x = position.x;
	m_Position.z = position.y;
}

void FpsCameraController::MoveRelative(const Float3& movement)
{
	const float yawRad = m_Yaw * Constants::TO_RAD;
	const float yawCos = cosf(yawRad);
	const float yawSin = sinf(yawRad);
	constexpr float xy = 0;
	const float xx = yawCos;
	const float xz = yawSin;

	m_Position += Float3{ xx, xy, xz } *movement.x;
	m_Position += Float3{ -xz, xy, xx } *movement.z;
	m_Position.y += movement.y;
}

Float2 FpsCameraController::GetXzForward() const
{
	const float yawRad = m_Yaw * Constants::TO_RAD;
	return { sinf(yawRad), cosf(yawRad) };
}

Float3 FpsCameraController::GetXzForward3() const
{
	const float yawRad = m_Yaw * Constants::TO_RAD;
	return { sinf(yawRad), 0, cosf(yawRad) };
}

Transform FpsCameraController::GetTransform() const
{
	const float pitchRad = m_Pitch * Constants::TO_RAD;
	const float yawRad = m_Yaw * Constants::TO_RAD;

	return Transform{
		m_Position,
		Quaternion::FromAxis({0,1,0},-yawRad) * Quaternion::FromAxis({1,0,0},pitchRad) };
}
