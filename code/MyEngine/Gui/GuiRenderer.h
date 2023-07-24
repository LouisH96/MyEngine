#pragma once
#include "Rendering/Renderers/NdcRectRenderer.h"
#include "Rendering/Structs/InstanceTypes.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace App
	{
		struct ResizedEvent;
	}

	namespace Gui
	{
		class GuiRenderer
		{
		public:
			//---| Constructors/Destructor |---
			GuiRenderer();

			//---| Operations |---
			void OnCanvasResized(const App::ResizedEvent& event);
			void Render();

			void Remove(int id);
			int Add(const Float2& pivot, const Float2& offset, const Float2& size, const Float3& color);
			int AddCenterBottom(const Float2& offset, const Float2& size, const Float3& color);

			int GetHovered() const;
			int GetClicked() const;

			void SetColor(int id, const Float3& color);
			void SetOffsetX(int id, float xPixels);
			void SetOffsetY(int id, float yPixels);

		private:
			using Vertex = Rendering::V_Pos2;
			using Instance = Rendering::I_Pos2Col;

			Rendering::NdcRectRenderer<Vertex, Instance> m_Renderer;
		};
	}
}
