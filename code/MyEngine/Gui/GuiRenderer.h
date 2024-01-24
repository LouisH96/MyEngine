#pragma once
#include "Rendering/Renderers/NdcRectRenderer.h"
#include "Rendering/Structs/InstanceTypes.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	struct UiRect;

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

			void Remove(unsigned id);
			void Remove(int id);
			int Add(const Float2& pivot, const Float2& offset, const Float2& size, const Float3& color);
			int Add(const UiRect& uiRect, const Float3& color);
			int AddCenterBottom(const Float2& offset, const Float2& size, const Float3& color);

			int GetHovered() const;
			int GetClicked() const;
			bool IsUnderMouse(int id) const;

			void SetColor(int id, const Float3& color);
			void SetOffsetX(int id, float xPixels);
			void SetOffsetY(int id, float yPixels);
			void SetHeight(int id, float newHeight);

			Float2 GetSize(int id) const;
			float GetWidth(int id) const;
			float GetHeight(int id) const;

			void GetBoundsNdc(int id, Float2& leftBot, Float2& rightTop);

		private:
			using Vertex = Rendering::V_Pos2;
			using Instance = Rendering::I_Pos2Col;

			Rendering::NdcRectRenderer<Vertex, Instance> m_Renderer;
		};
	}
}
