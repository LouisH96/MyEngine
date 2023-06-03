#pragma once
#include "Math/RectInt.h"
#include "Rendering/State/Shader.h"
#include "Rendering/State/VertexList.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace Gui
	{
		class GuiRenderer
		{
		public:
			GuiRenderer();

			void OnCanvasResize(const Int2& newSize);
			void Render();

			//anchor left-bottom
			void Add(const RectInt& rectangle, const Float3& color);

		private:
			using Vertex = Rendering::V_Pos2Col;
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;

			Int2 m_CanvasSize;

			Rendering::VertexList<Vertex> m_Vertices;
		};
	}
}
