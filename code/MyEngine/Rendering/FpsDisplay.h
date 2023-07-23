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

		class FpsDisplay
		{
		public:
			FpsDisplay();
			~FpsDisplay() = default;
			
			void Render();
			void SetFps(int fps) { m_Fps = fps; }

		private:
			//---| MEMBERS |---
			int m_Fps{};
			int m_TextId{};

			void SetText();
		};
	}
}
