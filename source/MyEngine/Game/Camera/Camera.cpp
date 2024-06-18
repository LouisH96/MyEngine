#include "pch.h"
#include "Camera.h"

#include "App/ResizedEvent.h"
#include "Math/Constants.h"
#include "Rendering/Canvas.h"

Camera::Camera(Int2 windowSize, float fov, float nearPlane, float farPlane)
	: m_World{ Float4X4::GetIdentity() }
	, m_View{ Float4X4::GetIdentity() }
	, m_Near{ nearPlane }
	, m_Far{ farPlane }
{
	SetFieldOfView(fov);
	m_AspectRatio = static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y);
	UpdateProjectionMatrix();
}

void Camera::OnCanvasResized(const App::ResizedEvent& event)
{
	m_AspectRatio = static_cast<float>(event.NewSize.x) / static_cast<float>(event.NewSize.y);
	UpdateProjectionMatrix();
}

void Camera::Update()
{
	const Float3 right{ m_World.GetRow0().Xyz() };
	const Float3 up{ m_World.GetRow1().Xyz() };
	const Float3 forward{ m_World.GetRow2().Xyz() };
	const Float3 invPos{ -m_World.GetRow3().Xyz() };

	m_View.SetCol0(right);
	m_View.SetCol1(up);
	m_View.SetCol2(forward);
	m_View.SetRow3({
		right.Dot(invPos),
		up.Dot(invPos),
		forward.Dot(invPos)
		});

	m_ViewProjection = m_View * m_Projection;
	m_Position = m_World.GetRow3().Xyz();
}

void Camera::SetFieldOfView(float angle)
{
	m_TanHalfFov = tan(angle / 2 * Constants::TO_RAD);
	UpdateProjectionMatrix();
}

void Camera::SetPosition(const Float3& position)
{
	m_World.SetRow3(position);
}

void Camera::SetRight(const Float3& right)
{
	m_World.SetRow0(right);
}

void Camera::SetUp(const Float3& up)
{
	m_World.SetRow1(up);
}

void Camera::SetForward(const Float3& forward)
{
	m_World.SetRow2(forward);
}

/**
 * \brief in radians
 */
void Camera::SetRotation(float pitch, float yaw)
{
	const float pitchCos = cosf(pitch);
	const float pitchSin = sinf(pitch);

	const float yawCos = cosf(yaw);
	const float yawSin = sinf(yaw);

	m_World.SetCol0({ yawCos, -yawSin * pitchSin, -yawSin * pitchCos });
	m_World.SetCol1({ 0, pitchCos, -pitchSin });
	m_World.SetCol2({ yawSin, yawCos * pitchSin, yawCos * pitchCos });
}

const Float3& Camera::GetRight() const
{
	return m_World.GetRow0().Xyz();
}

const Float3& Camera::GetUp() const
{
	return m_World.GetRow1().Xyz();
}

const Float3& Camera::GetForward() const
{
	return m_World.GetRow2().Xyz();
}

float Camera::GetHalfFov() const
{
	return atan(m_TanHalfFov);
}

Ray Camera::GetMouseRay(float rayLength)
{
	return GetRay(Globals::pMouse->GetPos(), rayLength);
}

Ray Camera::GetRay(const Int2& pixel, float rayLength) const
{
	constexpr float depth{ .065f };
	const float verScale{ depth * m_TanHalfFov };
	const float horScale{ verScale * m_AspectRatio };
	const float y{ (static_cast<float>(pixel.y) / static_cast<float>(Globals::pCanvas->GetSize().y) * 2.f - 1.f) };
	const float x{ (static_cast<float>(pixel.x) / static_cast<float>(Globals::pCanvas->GetSize().x) * 2.f - 1.f) };

	const Float3 forward{ GetForward() };
	const Float3 right{ GetRight() };
	const Float3 up{ GetUp() };
	Float3 direction{ forward };

	const float yaw{ atanf(m_TanHalfFov * m_AspectRatio * x) };
	const float pitch{ atanf(m_TanHalfFov * y * cos(yaw)) };
	Float3 pitchAxis{ right };

	Quaternion rotation{ Quaternion::FromAxis(up, yaw) };
	rotation.RotatePoint(direction);
	rotation.RotatePoint(pitchAxis);
	rotation = Quaternion::FromAxis(pitchAxis, pitch);
	rotation.RotatePoint(direction);

	return { GetPosition() + forward * depth + right * x * horScale - up * y * verScale, direction, rayLength };
}

void Camera::UpdateProjectionMatrix()
{
	const float a = m_Far / (m_Far - m_Near);
	const float b = -(m_Far * m_Near) / (m_Far - m_Near);
	m_Projection = {
		{1 / (m_AspectRatio * m_TanHalfFov), 0,0,0},
		{0,1 / m_TanHalfFov, 0,0},
		{0,0,a,b},
		{0,0,1,0}
	};
}
