#pragma once
#include "DataStructures/InvalidateList.h"
#include "Rendering/Buffers/InvalidateBuffer.h"

namespace MyEngine
{
	namespace NewUi
	{
		class NewUiShapeRenderer
		{
		public:
			NewUiShapeRenderer();

			void OnCanvasResized(const App::ResizedEvent& event);
			void Render();

			//should be in a Drawer object
			unsigned Rect(Float2 leftBot,Float2 size, const Float3& color);
			unsigned Line(Float2 begin, Float2 end, float thickness, const Float3& color);
			void Remove(unsigned id);

		private:
			using Vertex = Rendering::V_Pos2Col;
			struct Shape
			{
				List<unsigned> Triangles{};

				bool IsValid() const { return !Triangles.IsEmpty(); }
				void Invalidate() { Triangles.Clear(); }
			};

			static constexpr ModelTopology TOPOLOGY{ ModelTopology::TriangleList };
			Rendering::Shader m_Shader;
			Rendering::InputLayout m_InputLayout;

			InvalidateList<Shape> m_Shapes;
			Rendering::InvalidateBuffer<Vertex> m_Vertices;

			Float2 m_ToNdcMultiplier;

		};
	}
}
