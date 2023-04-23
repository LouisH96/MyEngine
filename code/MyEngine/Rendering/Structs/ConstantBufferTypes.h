#pragma once
#include "Math/Float3.h"
#include <DirectXMath.h>

namespace MyEngine
{
	namespace Game
	{
		class Transform;
		class FocusPointCamera;
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
			CB_CamMat(const Game::FocusPointCamera& cam);
		};
		struct CB_CamMatPos
		{
			DirectX::XMFLOAT4X4 CameraMatrix;
			Math::Float3 CameraPos;
			float padding = 0;

			CB_CamMatPos() = default;
			explicit CB_CamMatPos(const Game::FocusPointCamera& cam);
			CB_CamMatPos(const Game::FocusPointCamera& cam, const Game::Transform& transforms);
		};
		struct CB_ModelBuffer
		{
			DirectX::XMFLOAT4X4 ModelMatrix{};
			DirectX::XMFLOAT4X4 InvTransposeModelMatrix{};

			CB_ModelBuffer();
			explicit CB_ModelBuffer(const Game::Transform& transform);
		};
	}
}
