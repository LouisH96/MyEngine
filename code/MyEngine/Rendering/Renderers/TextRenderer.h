#pragma once
#include <Rendering/Renderers/RendererFactory.h>
#include "Rendering/State/BlendState.h"
#include <Rendering/Renderers/TextureRenderer.h>

namespace MyEngine
{
	namespace Game
	{
		class Camera;
	}

	namespace Rendering
	{
		class Gpu;

		class TextRenderer
		{
		public:
			TextRenderer(Gpu& gpu, Game::Camera& camera);
			~TextRenderer();

			void Render() const;

		private:
			TextureRenderer<V_PosUv, CB_CamMat>* m_pTextureRenderer;
		};
	}
}
