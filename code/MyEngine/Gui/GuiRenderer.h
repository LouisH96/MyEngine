#pragma once
#include "Math/Rects.h"
#include "Rendering/Buffers/Buffer.h"
#include "Rendering/Buffers/InvalidateBuffer.h"
#include "Rendering/DrawData/VertexList.h"
#include "Rendering/State/DepthStencilState.h"
#include "Rendering/State/Shader.h"
#include "Rendering/Structs/Instances.h"
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
			void OnCanvasResize(const Int2& newSize);
			void Render();

			void Remove(ElementId id);
			ElementId Add(const Float2& pivot, const Float2& offset, const Float2& size, const Float3& color);
			ElementId AddCenterBottom(const Float2& offset, const Float2& size, const Float3& color);

			ElementId GetElementUnderMouse() const;
			void SetColor(ElementId id, const Float3& color);
			void SetOffsetX(ElementId id, float xPixels);
			void SetOffsetY(ElementId id, float yPixels);

		private:
			using Vertex = Rendering::V_Pos2;
			using Instance = Rendering::I_Pos2Col;

			Rendering::DepthStencilState m_DepthStencilState;
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;

			Float2 m_InvCanvasSize;
			Array<Float2> m_Pivots{}; //Elements point at pivot is attached to screens point at pivot

			Rendering::Buffer<Vertex> m_Vertices;
			Rendering::InvalidateBuffer<Instance> m_Instances;
			D3D11_PRIMITIVE_TOPOLOGY m_Topology;

			static bool IsEmpty(const Instance& instance);
			static void SetEmpty(Instance& instance);
		};
	}
}
