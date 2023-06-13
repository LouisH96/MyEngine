#pragma once
#include "Math/Rects.h"
#include "Rendering/DrawData/VertexList.h"
#include "Rendering/State/DepthStencilState.h"
#include "Rendering/State/Shader.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace Gui
	{
		class GuiRenderer
		{
		public:
			//---| Types |---
			class ElementId
			{
			public:
				ElementId() = default;
				explicit ElementId(int id);
				int GetId() const { return m_Id; }
				bool IsValid() const { return m_Id >= 0; }
				bool operator==(const ElementId& other) const;
			private:
				int m_Id;
			};

			//---| Constructors/Destructor |---
			GuiRenderer();

			//---| Operations |---
			const Int2& GetCanvasSize() const { return m_CanvasSize; }
			void OnCanvasResize(const Int2& newSize);
			void Render();

			ElementId AddLeftBottom(const RectInt& rectangle, const Float3& color);
			ElementId AddCenterBottom(const RectInt& rectangle, const Float3& color);

			ElementId GetUnderMouse() const;
			void ChangeColor(ElementId id, const Float3& color);
			void ChangePositionX(ElementId id, int x);

		private:
			static constexpr int VERTICES_PER_RECT = 6;
			using Vertex = Rendering::V_Pos2Col;
			Rendering::DepthStencilState m_DepthStencilState;
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;

			Int2 m_CanvasSize;
			Rendering::VertexList<Vertex> m_Vertices;
			int m_CenterBottomAnchoredIdx; //begin of vertices anchored to center-bottom (before is anchored to left-bottom)

			void Add(int idx, const RectFloat& rect, const Float3& color);
			void Replace(int idx, const RectFloat& rect);

			Float2 MouseInClip() const;
			int ToVertexId(ElementId id) const;
			static float ToClipAlignMin(int screenPos, float screenSize);
			static float ToClipAlignCenter(int screenPos, float screenSize);
			static float SizeToClip(int size, float screenSize);
			static Float2 SizeToClip(const Int2& size, const Float2& screenSize);
		};
	}
}
