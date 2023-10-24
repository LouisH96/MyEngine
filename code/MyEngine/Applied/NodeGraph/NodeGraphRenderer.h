#pragma once
#include "Node.h"
#include "Applied/2D/Camera2D.h"
#include "Rendering/Buffers/Buffer.h"
#include "Rendering/Buffers/IdxBuffer.h"

namespace MyEngine
{
	namespace Applied
	{
		class NodeGraphRenderer
		{
		public:
			using CameraBuffer = Camera2D::CameraBuffer;

			NodeGraphRenderer();

			void UpdateData(PtrRangeConst<Node> nodes);
			void Render(const Camera2D& camera);

		private:
			Rendering::ConstantBuffer<CameraBuffer> m_CameraBuffer;
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;

			Rendering::Buffer<Node::Vertex> m_Vertices;
			Rendering::IdxBuffer m_Indices;

			unsigned m_NrIndices;
			unsigned m_NrVertices;

			static void WriteData(PtrRangeConst<Node> nodes, int* pIndices, Node::Vertex* pVertices);
		};
	}
}
