#include "pch.h"
#include "ConstantBufferTypes.h"

#include "Transform/Transform.h"
#include "Game/Camera/Camera.h"

Rendering::CB_CamMat::CB_CamMat(const Camera& camera)
	: CameraMatrix{ camera.GetViewProjection() }
{
}

Rendering::CB_CamMat::CB_CamMat(const Float4X4& viewProjection)
	: CameraMatrix{ viewProjection }
{
}

Rendering::CB_CamMat::CB_CamMat(const Float3& position, const Float4X4& viewProjection)
	: CameraMatrix{ viewProjection }
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Camera& camera)
	: CameraMatrix{ camera.GetViewProjection() }
	, CameraPos{ camera.GetPosition() }
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Float3& position, const Float4X4& matrix)
	: CameraMatrix{ matrix }
	, CameraPos{ position }
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Float3& position, const Float4X4& viewProjection,
	const Game::Transform& transform)

	: CameraMatrix{ transform.AsMatrix() * viewProjection }
	, CameraPos{ position }
{
}

Rendering::CB_CamPos::CB_CamPos(const Float3& position)
	: CameraPos{ position }
{
}

Rendering::CB_ModelBuffer::CB_ModelBuffer(const Game::Transform& transform)
	: ModelMatrix{ transform.AsMatrix() }
{
}
