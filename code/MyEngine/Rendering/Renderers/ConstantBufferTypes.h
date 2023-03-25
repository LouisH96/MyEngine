#pragma once
#include "Math/Float3.h"
#include <DirectXMath.h>

namespace MyEngine
{
	namespace Game
	{
		namespace Camera
		{
			class Camera;
		}
	}
}

namespace MyEngine
{
	namespace Rendering
	{
		struct CB_CamMat
		{
			DirectX::XMFLOAT4X4 CameraMatrix;

			CB_CamMat() = default;
			CB_CamMat(const Game::Camera::Camera& cam);
		};
		struct CB_CamMatPos
		{
			DirectX::XMFLOAT4X4 CameraMatrix;
			Math::Float3 CameraPos;
			float padding = 0;

			CB_CamMatPos() = default;
			CB_CamMatPos(const Game::Camera::Camera& cam);
		};
	}
}
