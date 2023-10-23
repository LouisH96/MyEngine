#pragma once
#include "NodeGraphRenderer.h"
#include "Applied/2D/Camera2D.h"
#include "Applied/2D/Camera2DController.h"

namespace MyEngine
{
	namespace Applied
	{
		class NodeGraph
		{
		public:
			NodeGraph();

			void Update();
			void Render();

		private:
			Camera2D m_Camera;
			Camera2DController m_CameraController;
			NodeGraphRenderer m_Renderer;
		};
	}
}
