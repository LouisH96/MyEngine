#pragma once
#include "NodeGrabbing.h"
#include "NodeGraphRenderer.h"
#include "Applied/2D/Camera2D.h"
#include "Applied/2D/Camera2DController.h"
#include "DataStructures/InvalidateList.h"

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
			void OnCanvasResized(const App::ResizedEvent& event);

			int Add(const Node& node);
			void SetParent(unsigned childId, unsigned parentId);

			const InvalidateList<Node>& GetNodes() const { return m_Nodes; }
			InvalidateList<Node>& GetNodes() { return m_Nodes; }

		private:
			Camera2D m_Camera;
			Camera2DController m_CameraController;
			NodeGraphRenderer m_Renderer;

			InvalidateList<Node> m_Nodes;

			//grabbing
			NodeGrabbing m_NodeGrabbing{};
		};
	}
}
