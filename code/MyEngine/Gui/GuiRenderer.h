#pragma once
#include "Math/RectFloat.h"
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

			void AddLeftBottom(const RectInt& rectangle, const Float3& color);
			void AddCenterBottom(const RectInt& rectangle, const Float3& color);

		private:
			static constexpr int VERTICES_PER_RECT = 6;
			using Vertex = Rendering::V_Pos2Col;
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;

			Int2 m_CanvasSize;
			Rendering::VertexList<Vertex> m_Vertices;
			int m_CenterBottomAnchoredIdx; //begin of vertices anchored to center-bottom (before is anchored to left-bottom)

			void Add(int idx, const RectFloat& rect, const Float3& color);
			void Replace(int idx, const RectFloat& rect);

			static float ToClipAlignMin(int screenPos, float screenSize);
			static float ToClipAlignCenter(int screenPos, float screenSize);
			static float SizeToClip(int size, float screenSize);
			static Float2 SizeToClip(const Int2& size, const Float2& screenSize);
		};
	}
}
