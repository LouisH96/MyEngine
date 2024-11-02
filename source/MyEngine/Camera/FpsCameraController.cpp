#include "pch.h"
#include "FpsCameraController.h"

#include "Camera.h"

FpsCameraController::FpsCameraController()
	: FpsCameraController{ *Globals::pCamera }
{
}

FpsCameraController::FpsCameraController(Camera& camera)
	: m_Camera{ camera }
	, m_LookAroundSpeed{ 0.25 * Constants::TO_RAD, 0.25 * Constants::TO_RAD }
	, m_Pitch{ 0 }
	, m_Yaw{ 0 }
{
}

void FpsCameraController::Update()
{
	const Int2& mouseDelta = Globals::pMouse->GetMovement();
	m_Yaw += static_cast<float>(mouseDelta.x) * m_LookAroundSpeed.x;
	m_Pitch += static_cast<float>(mouseDelta.y) * m_LookAroundSpeed.y;

	constexpr float max{ Constants::PI_DIV2_S };
	if (m_Pitch > max)
		m_Pitch = max;
	else if (m_Pitch < -max)
		m_Pitch = -max;

	//CAMERA
	m_Camera.SetRotation(m_Pitch, m_Yaw);
	m_Camera.SetPosition(m_Position);
}

void FpsCameraController::SetPositionXz(const Float2& position)
{
	m_Position.x = position.x;
	m_Position.z = position.y;
}

void FpsCameraController::MoveRelative(const Float3& movement)
{
	m_Position += GetRelativeMovement(movement);
}

Float2 FpsCameraController::GetXzForward() const
{
	const float yawRad = m_Yaw;
	return { sinf(yawRad), cosf(yawRad) };
}

Float3 FpsCameraController::GetXzForward3() const
{
	const float yawRad = m_Yaw;
	return { sinf(yawRad), 0, cosf(yawRad) };
}

Transform FpsCameraController::GetTransform() const
{
	return Transform{
		m_Position,
		Quaternion::FromAxis({0,1,0}, m_Yaw) * Quaternion::FromAxis({1,0,0},m_Pitch) };
}

Float3 FpsCameraController::GetRelativeMovement(const Float2& movement) const
{
	return m_Camera.GetRightXz() * movement.x
		+ m_Camera.GetForwardXz() * movement.y;
}

Float3 FpsCameraController::GetRelativeMovement(const Float3& movement) const
{
	return m_Camera.GetRightXz() * movement.x
		+ m_Camera.GetForwardXz() * movement.z
		+ Float3{ 0,movement.y, 0 };
}
