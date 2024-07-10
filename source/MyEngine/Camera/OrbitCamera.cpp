#include "pch.h"
#include "OrbitCamera.h"

#include <Windows.h>
#include <App/Win32/Mouse.h>
#include <Camera/Camera.h>
#include <Math/Constants.h>

using namespace App::Win32;

OrbitCamera::OrbitCamera()
	: OrbitCamera{ *Globals::pCamera }
{
}

OrbitCamera::OrbitCamera(Camera& camera)
	: m_Camera(camera)
	, m_ScrollSpeed(-1.f)
	, m_HorizontalSpeed(1.f)
	, m_VerticalSpeed{ 1.f }
	, m_Pitch{ 50 }
	, m_Yaw{ 0 }
	, m_Distance{ 5 }
{
}

void OrbitCamera::Update()
{
	UpdateRotation();
	UpdateMovement();
	UpdateScroll();

	FinishUpdate();
}

void OrbitCamera::UpdateMovement()
{
	const float maxVerSpeed = m_VerticalSpeed * Globals::DeltaTime;
	float horSpeed = m_HorizontalSpeed * Globals::DeltaTime;
	if (maxVerSpeed != 0 || horSpeed != 0)
	{
		Float3 translation;
		translation.x = static_cast<float>(Globals::pKeyboard->IsDown('D') - Globals::pKeyboard->IsDown('Q'));
		translation.y = static_cast<float>(Globals::pKeyboard->IsDown('E') - Globals::pKeyboard->IsDown('A')) * maxVerSpeed;
		translation.z = static_cast<float>(Globals::pKeyboard->IsDown('Z') - Globals::pKeyboard->IsDown('S'));

		//multiply with speed
		if (translation.x != 0 && translation.z != 0) horSpeed *= Constants::DIVSQR2;
		translation.x *= horSpeed;
		translation.z *= horSpeed;
		MoveRelative(translation);
	}
}

void OrbitCamera::UpdateRotation()
{
	if (Globals::pMouse->IsRightBtnDown())
		MouseRotation();
}

void OrbitCamera::UpdateScroll()
{
	m_Distance += Globals::pMouse->GetScroll() * m_ScrollSpeed;
}

void OrbitCamera::FinishUpdate()
{
	m_Camera.SetRotation(m_Pitch * Constants::TO_RAD, m_Yaw * Constants::TO_RAD);
	m_Camera.SetPosition(m_Camera.GetForward() * -m_Distance + m_FocusPoint);
}

void OrbitCamera::SetFocusPoint(const Float3& position)
{
	m_FocusPoint = position;
}

void OrbitCamera::SetPitch(float pitch)
{
	m_Pitch = pitch;
}

void OrbitCamera::SetYaw(float yaw)
{
	m_Yaw = yaw;
}

void OrbitCamera::SetDistance(float distance)
{
	m_Distance = distance;
}

void OrbitCamera::MoveRelative(const Float3& movement)
{
	const float yawRad = m_Yaw * Constants::TO_RAD;
	const float yawCos = cosf(yawRad);
	const float yawSin = sinf(yawRad);
	constexpr float xy = 0;
	const float xx = yawCos;
	const float xz = yawSin;

	m_FocusPoint += Float3{ xx, xy, xz } *movement.x;
	m_FocusPoint += Float3{ -xz, xy, xx } *movement.z;
	m_FocusPoint.y += movement.y;
}

Float3 OrbitCamera::GetCameraPosition() const
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

void OrbitCamera::MouseRotation()
{
	constexpr float radiansPerPixel = Constants::PI / 10;
	const Int2& mouseDelta = Globals::pMouse->GetMovement();
	m_Yaw += static_cast<float>(mouseDelta.x) * -radiansPerPixel;
	m_Pitch += static_cast<float>(mouseDelta.y) * radiansPerPixel;
}
