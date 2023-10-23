#pragma once
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
			using Vertex = Rendering::V_Pos2Col;
			using CameraBuffer = Camera2D::CameraBuffer;
			static constexpr ModelTopology TOPOLOGY{ ModelTopology::TriangleListIdx };

			NodeGraphRenderer();

			void Render(const Camera2D& camera);

		private:
			Rendering::ConstantBuffer<CameraBuffer> m_CameraBuffer;
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;

			Rendering::Buffer<Vertex> m_Vertices;
			Rendering::IdxBuffer m_Indices;

		};
	}
}
