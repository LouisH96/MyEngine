#pragma once
#include "Math/Float3.h"
#include <DirectXMath.h>

namespace MyEngine
{
	namespace Game
	{
		class Camera;
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
			DirectX::XMFLOAT4X4 CameraMatrix{};

			CB_CamMat() = default;
			explicit CB_CamMat(const DirectX::XMMATRIX& viewProjection);
			explicit CB_CamMat(const DirectX::XMFLOAT4X4& viewProjection);
			explicit CB_CamMat(const Math::Float3& position ,const DirectX::XMMATRIX& viewProjection);
		};
		struct CB_CamMatPos
		{
			DirectX::XMFLOAT4X4 CameraMatrix{};
			Math::Float3 CameraPos;
			float padding = 0;

			explicit CB_CamMatPos(const Math::Float3& position, const DirectX::XMFLOAT4X4& matrix);
			explicit CB_CamMatPos(const Math::Float3& position, const DirectX::XMMATRIX& matrix);
			explicit CB_CamMatPos(const Math::Float3& position, const DirectX::XMMATRIX& viewProjection, const Game::Transform& transform);
		};
		struct CB_ModelBuffer
		{
			DirectX::XMFLOAT4X4 ModelMatrix{};
			DirectX::XMFLOAT4X4 InvTransposeModelMatrix{};

			CB_ModelBuffer() = default;
			explicit CB_ModelBuffer(const Game::Transform& transform);
		};
	}
}
