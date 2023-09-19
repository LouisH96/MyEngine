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

			const CameraBuffer& GetCameraBuffer() const { return m_Buffer; }

		private:
			DxMatrix m_World{};
			CameraBuffer m_Buffer;

			float m_Zoom{ 1.f };
		};
	}
}
