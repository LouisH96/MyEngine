#include "pch.h"
#include "ConstantBufferTypes.h"

#include "Transform/Transform.h"
#include "Camera/Camera.h"

using namespace Rendering;

CB_CamMat::CB_CamMat(const Camera& camera)
	: CameraMatrix{ camera.GetViewProjection() }
{
}

CB_CamMat::CB_CamMat(const Float4X4& viewProjection)
	: CameraMatrix{ viewProjection }
{
}

CB_CamMat::CB_CamMat(const Float3&, const Float4X4& viewProjection)
	: CameraMatrix{ viewProjection }
{
}

CB_CamMatPos::CB_CamMatPos(const Camera& camera)
	: CameraMatrix{ camera.GetViewProjection() }
	, CameraPos{ camera.GetPosition() }
{
}

CB_CamMatPos::CB_CamMatPos(const Float3& position, const Float4X4& matrix)
	: CameraMatrix{ matrix }
	, CameraPos{ position }
{
}

CB_CamMatPos::CB_CamMatPos(const Float3& position, const Float4X4& viewProjection,
	const Transform& transform)

	: CameraMatrix{ transform.AsMatrix() * viewProjection }
	, CameraPos{ position }
{
}

CB_CamPos::CB_CamPos(const Float3& position)
	: CameraPos{ position }
{
}

CB_CamMatPosFor::CB_CamMatPosFor(const Camera& camera)
	: CameraMatrix{ camera.GetViewProjection() }
	, CameraPos{ camera.GetPosition() }
	, CameraForward{ camera.GetForward() }
{
}

CB_CamMatPosFor::CB_CamMatPosFor(const Float3& position, const Float3& forward, const Float4X4& matrix)
	: CameraMatrix{ matrix }
	, CameraPos{ position }
	, CameraForward{ forward }
{
}

CB_Cam_View_ViewProj_Pos_For::CB_Cam_View_ViewProj_Pos_For(const Camera& camera)
	: View{ camera.GetView() }
	, ViewProjection{ camera.GetViewProjection() }
	, Position{ camera.GetPosition() }
	, Forward{ camera.GetForward() }
{
}

CB_ModelBuffer::CB_ModelBuffer(const Transform& transform)
	: ModelMatrix{ transform.AsMatrix() }
{
}

CB_ModelBuffer::CB_ModelBuffer(const Float4X4& matrix)
	: ModelMatrix{ matrix }
{
}
