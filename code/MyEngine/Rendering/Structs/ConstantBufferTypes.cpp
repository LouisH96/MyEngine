#include "pch.h"
#include "ConstantBufferTypes.h"

#include "Game/Transform.h"

using namespace DirectX;

Rendering::CB_CamMat::CB_CamMat(const Float4X4& viewProjection)
	: CameraMatrix{ viewProjection }
{
}

Rendering::CB_CamMat::CB_CamMat(const Math::Float3& position, const Math::Float4X4& viewProjection)
	: CameraMatrix{ viewProjection }
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Math::Float3& position, const Math::Float4X4& matrix)
	: CameraMatrix{ matrix }
	, CameraPos{ position }
{
}

Rendering::CB_CamMatPos::CB_CamMatPos(const Math::Float3& position, const Math::Float4X4& viewProjection,
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
	, InvTransposeModelMatrix{ transform.GetTransposeInverse() }
{
}
