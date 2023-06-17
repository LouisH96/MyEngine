#pragma once

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
			Float4X4 CameraMatrix{};

			CB_CamMat() = default;
			explicit CB_CamMat(const Float4X4& viewProjection);
			explicit CB_CamMat(const Float3& position ,const Float4X4& viewProjection);
		};
		struct CB_CamMatPos
		{
			Float4X4 CameraMatrix{};
			Float3 CameraPos;
			float padding = 0;

			explicit CB_CamMatPos(const Float3& position, const Float4X4& matrix);
			explicit CB_CamMatPos(const Float3& position, const Float4X4& viewProjection, const Transform& transform);
		};
		struct CB_CamPos
		{
			Float3 CameraPos;
			float padding = 0;

			explicit CB_CamPos(const Float3& position);
		};
		struct CB_ModelBuffer
		{
			Float4X4 ModelMatrix{};

			CB_ModelBuffer() = default;
			explicit CB_ModelBuffer(const Transform& transform);
		};
	}
}
