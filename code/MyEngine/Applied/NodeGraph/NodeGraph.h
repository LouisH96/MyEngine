#pragma once
#include "NodeGrabbing.h"
#include "NodeGraphFontRenderer.h"
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

			int Add(const Float2& leftTop, float contentHeight, const Float3& color, const std::string& text);
			void SetParent(unsigned childId, unsigned parentId);
			void AutoStructure();

			const InvalidateList<Node>& GetNodes() const { return m_Nodes; }
			InvalidateList<Node>& GetNodes() { return m_Nodes; }

			void SetPos(unsigned nodeIdx, Float2 position, const Float2& pivot);
			Float2 GetPos(unsigned nodeIdx, const Float2& pivot);
			const Node& GetNode(unsigned nodeIdx) const;
			unsigned GetDepth(unsigned nodeIdx) const;
			unsigned GetRootNodeId(unsigned nodeId) const;

		private:
			Camera2D m_Camera;
			Camera2DController m_CameraController;
			NodeGraphRenderer m_Renderer;
			NodeGraphFontRenderer m_FontRenderer;

			InvalidateList<Node> m_Nodes;

			//grabbing
			NodeGrabbing m_NodeGrabbing{};

			static void MakeDemoGraph(NodeGraph& graph);
		};
	}
}
