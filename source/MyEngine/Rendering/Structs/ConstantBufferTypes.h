// ReSharper disable CppNonExplicitConvertingConstructor
#pragma once

namespace MyEngine
{
	class Camera;
	class Transform;

	namespace Rendering
	{
		struct CB_CamMat
		{
			Float4X4 CameraMatrix{};

			CB_CamMat() = default;
			CB_CamMat(const Camera& camera);
			explicit CB_CamMat(const Float4X4& viewProjection);
			explicit CB_CamMat(const Float3& position, const Float4X4& viewProjection);
		};
		struct CB_CamMatPos
		{
			Float4X4 CameraMatrix{};
			Float3 CameraPos;
			float padding = 0;

			CB_CamMatPos(const Camera& camera);
			explicit CB_CamMatPos(const Float3& position, const Float4X4& matrix);
			explicit CB_CamMatPos(const Float3& position, const Float4X4& viewProjection, const Transform& transform);
		};
		struct CB_CamPos
		{
			Float3 CameraPos;
			float padding = 0;

			explicit CB_CamPos(const Float3& position);
		};
		struct CB_Cam_View_ViewProj_Pos_For
		{
			Float4X4 View;
			Float4X4 ViewProjection;
			Float3 Position;
			float Padding1{};
			Float3 Forward;
			float Padding2{};

			CB_Cam_View_ViewProj_Pos_For(const Camera& camera);
		};
		struct CB_ModelBuffer
		{
			Float4X4 ModelMatrix{};

			CB_ModelBuffer() = default;
			explicit CB_ModelBuffer(const Transform& transform);
			explicit CB_ModelBuffer(const Float4X4& matrix);
		};
		struct CB_BonesBuffer
		{
			static constexpr unsigned MAX_BONES = 1024;
			Float4X4 BoneTransforms[MAX_BONES];
		};
	}
}
