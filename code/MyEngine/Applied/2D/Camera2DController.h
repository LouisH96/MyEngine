#pragma once

namespace MyEngine
{
	namespace Applied
	{
		class Camera2DController
		{
		public:
			Camera2DController();

			void EarlyUpdate();

			void SetCamera(Camera2D* pCamera);

		private:
			Camera2D* m_pCamera;
			float m_ZoomSpeed{ .1f };
			float m_MoveSpeed{ .001f };
		};
	}
}
