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

			void IncreaseNrIndices(int amount);
			void IncreaseNrVertices(int amount);

		private:
			Rendering::ConstantBuffer<CameraBuffer> m_CameraBuffer;
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;

			Rendering::Buffer<Node::Vertex> m_Vertices;
			Rendering::IdxBuffer m_Indices;

			unsigned m_NrIndices{ 0 };
			unsigned m_NrVertices{ 0 };

			static void WriteData(PtrRangeConst<Node> nodes, int* pIndices, Node::Vertex* pVertices);

			static void WriteRectVertices(const Node& node, Node::Vertex*& pTarget);
			static void WriteRectIndices(const Node& node, int*& pTarget, unsigned offset);

			static void WriteConnectionVertices(const Node& node, const Node& parentNode, Node::Vertex*& pTarget);
			static void WriteConnectionIndices(int*& pTarget, unsigned offset);

			static void WriteRectVertices(Node::Vertex*& pTarget, const RectFloat& rect, const Float3& color);
		};
	}
}
