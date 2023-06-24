#include "pch.h"
#include "FocusPointCameraController.h"

#include <DirectXMath.h>
#include <Windows.h>
#include <App/Win32/Mouse.h>
#include <Game/Camera/Camera.h>
#include <Math/Constants.h>

using namespace App::Win32;
using namespace Math;

FocusPointCameraController::FocusPointCameraController()
	: FocusPointCameraController{ *Globals::pCamera }
{
}

FocusPointCameraController::FocusPointCameraController(Camera& camera)
	: m_Camera(camera)
	, m_ScrollSpeed(-1.f)
	, m_HorizontalSpeed(1.f)
	, m_VerticalSpeed{ 1.f }
	, m_Pitch{ 0 }
	, m_Yaw{ 0 }
	, m_Distance{ 1 }
{
}

void FocusPointCameraController::Update()
{
	//ROTATION
	if (Globals::pMouse->IsRightBtnDown())
		MouseRotation();

	//TRANSLATION
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

	//SCROLL
	m_Distance += Globals::pMouse->GetScroll() * m_ScrollSpeed;

	//UPDATE CAMERA
	m_Camera.SetRotation(m_Pitch * Constants::TO_RAD, m_Yaw * Constants::TO_RAD);
	m_Camera.SetPosition(m_Camera.GetForward() * -m_Distance + m_FocusPoint);
}

void FocusPointCameraController::SetFocusPoint(const Float3& position)
{
	m_FocusPoint = position;
}

void FocusPointCameraController::SetPitch(float pitch)
{
	m_Pitch = pitch;
}

void FocusPointCameraController::SetYaw(float yaw)
{
	m_Yaw = yaw;
}

void FocusPointCameraController::SetDistance(float distance)
{
	m_Distance = distance;
}

void FocusPointCameraController::MoveRelative(const Float3& movement)
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

Float3 FocusPointCameraController::GetCameraPosition() const
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

void FocusPointCameraController::MouseRotation()
{
	constexpr float radiansPerPixel = Constants::PI / 10;
	const Int2& mouseDelta = Globals::pMouse->GetMovement();
	m_Yaw += static_cast<float>(mouseDelta.x) * -radiansPerPixel;
	m_Pitch += static_cast<float>(mouseDelta.y) * radiansPerPixel;
}
