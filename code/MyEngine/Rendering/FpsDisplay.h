#pragma once
#include "Rendering/State/BlendState.h"
#include "Rendering/State/RasterizerState.h"
#include "Rendering/State/SamplerState.h"
#include "Rendering/State/Shader.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace Rendering
	{
		class Texture;
		class Mesh;
		class Image;
		class Canvas;
		class TextRenderer;
	}
}

namespace MyEngine
{
	namespace Rendering
	{
		class FpsDisplay
		{
		public:
			FpsDisplay(Gpu& gpu, Canvas& canvas, App::Win32::Window& window);
			~FpsDisplay();
			
			void Render();
			void OnWindowResized(DirectX::XMINT2 newSize);

			void SetFps(int fps) { m_Fps = fps; }

		private:
			//---| MEMBERS |---
			static constexpr int NR_NUMBERS = 10;
			Gpu& m_Gpu;
			Canvas& m_Canvas;
			BlendState m_BlendState;
			RasterizerState m_RasterizerState;
			SamplerState m_Sampler;
			Shader m_Shader;
			InputLayout m_InputLayout;

			Array<V_Pos2Uv> m_Vertices;
			Mesh* m_pMesh{};
			Texture* m_pTextures[NR_NUMBERS]{};
			Math::Float2 m_Sizes[NR_NUMBERS]{};
			Math::Float2 m_ScreenSpaceMargin{};
			int m_Fps{};

			static void SetPos(Array<V_Pos2Uv>& vertices, const Math::Float2& origin, const Math::Float2& size);
			static void InitVertices(Array<V_Pos2Uv>& vertices);
		};
	}
}
