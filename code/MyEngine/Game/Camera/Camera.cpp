#include "Camera.h"

#include "../../Logging/Logger.h"
#include <cmath>
#include "../../Math/Math.h"

MyEngine::Game::Camera::Camera::Camera(DirectX::XMINT2 size)
{
	SetFieldOfView(90.f);
	OnWindowResized(size);
}

void MyEngine::Game::Camera::Camera::Move(DirectX::XMFLOAT3 movement)
{
	using namespace DirectX;

	/*m_Matrix.m[3][0] -= movement.x;
	m_Matrix.m[3][1] -= movement.y;
	m_Matrix.m[3][2] -= movement.z;

	m_Matrix.m[3][3] -= movement.z;*/
}

void MyEngine::Game::Camera::Camera::Pitch(float addAngle)
{
	m_Pitch += addAngle;
	UpdateRotationMatrix();
}

void MyEngine::Game::Camera::Camera::Yaw(float addAngle)
{
	m_Yaw += addAngle;
	UpdateRotationMatrix();
}

void MyEngine::Game::Camera::Camera::OnWindowResized(DirectX::XMINT2 newSize)
{
	m_InvAspectRatio = static_cast<float>(newSize.y) / static_cast<float>(newSize.x);

	m_Matrix.m[0][0] = m_InvAspectRatio * m_FovValue;
}

void MyEngine::Game::Camera::Camera::SetFieldOfView(float angle)
{
	using namespace DirectX;
	m_FovValue = 1.f / tanf(XMConvertToRadians(angle / 2.f));

	m_Matrix.m[0][0] = m_FovValue * m_InvAspectRatio;
	m_Matrix.m[1][1] = m_FovValue;
}

void MyEngine::Game::Camera::Camera::UpdateRotationMatrix()
{
	const float pitchRad = m_Pitch * Math::TO_RAD;
	const float pitchCos = cosf(pitchRad);
	const float pitchSin = sinf(pitchRad);

	const float yawRad = m_Yaw * Math::TO_RAD;
	const float yawCos = cosf(yawRad);
	const float yawSin = sinf(yawRad);

	const float xx = yawCos;
	const float xy = 0;
	const float xz = yawSin;

	const float yx = -yawSin * pitchSin;
	const float yy = pitchCos;
	const float yz = yawCos * pitchSin;

	const float zx = -yawSin * pitchCos;
	const float zy = -pitchSin;
	const float zz = yawCos * pitchCos;

	m_World =
	{
		xx,xy,xz,0,
		yx,yy,yz,0,
		zx,zy,zz,0,
		-zx * m_FocusPointDistance,
		-zy * m_FocusPointDistance,
		-zz * m_FocusPointDistance,
		1
	};

	m_Proj =
	{
		m_InvAspectRatio * m_FovValue,0,0,0,
		0,m_FovValue,0,0,
		0,0,1,1,
		0,0,0,0
	};

	using namespace DirectX;
	XMMATRIX xm_world = XMLoadFloat4x4(&m_World);
	XMMATRIX xm_proj = XMLoadFloat4x4(&m_Proj);

	XMMATRIX xm_viewProj = XMMatrixMultiply(XMMatrixInverse(nullptr, xm_world), xm_proj);

	XMStoreFloat4x4(&m_Matrix, xm_viewProj);
}
