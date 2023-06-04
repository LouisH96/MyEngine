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

			const Int2& GetCanvasSize() const { return m_CanvasSize; }
			void OnCanvasResize(const Int2& newSize);
			void Render();

			int AddLeftBottom(const RectInt& rectangle, const Float3& color);
			int AddCenterBottom(const RectInt& rectangle, const Float3& color);

			int GetUnderMouse() const;

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


			Float2 MouseInClip() const;
			static float ToClipAlignMin(int screenPos, float screenSize);
			static float ToClipAlignCenter(int screenPos, float screenSize);
			static float SizeToClip(int size, float screenSize);
			static Float2 SizeToClip(const Int2& size, const Float2& screenSize);
		};
	}
}
