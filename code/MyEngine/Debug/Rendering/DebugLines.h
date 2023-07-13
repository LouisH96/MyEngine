#pragma once
#include "Rendering/DrawData/VertexList.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/Shader.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace Debug
	{
		class DebugLines
		{
		public:
			explicit DebugLines();

			void Render();

			void DrawLine(const Float3& begin, const Float3& end, const Float3& color);
			void DrawLine(const Float3* pData, unsigned count, const Float3& color);
			void DrawRay(const Float3& origin, const Float3& displacement, const Float3& color);

		private:
			using Vertex = Rendering::V_PosCol;

			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;

			Rendering::VertexList<Vertex> m_VertexList{};
		};
	}
}
