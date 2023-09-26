#pragma once

namespace MyEngine
{
	namespace Applied
	{
		class Camera2D
		{
		public:
			struct DxMatrix
			{
				//column-major
				Float4 Matrix[3];

				static DxMatrix Identity();
			};
			struct CameraBuffer
			{
				DxMatrix CameraView;
			};

			Camera2D();

			void OnCanvasResized(const App::ResizedEvent& event);
			void Update();

			void AddZoom(float amount);
			void ScaleZoom(float amount);
			float GetZoom() const { return m_Zoom; }
			void SetZoom(float zoom);

			void Move(const Float2& movement);
			void MoveScaled(const Float2& movement); //more movement if zoomed out

			Float2 GetWorldPos() const;
			Float2 GetMouseWorldPos() const;

			const CameraBuffer& GetCameraBuffer() const { return m_Buffer; }

		private:
			DxMatrix m_World{};
			CameraBuffer m_Buffer;

			float m_Zoom{ 1.f };
		};
	}
}
