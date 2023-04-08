#pragma once
#include <Rendering/Renderers/RendererFactory.h>
#include "Rendering/State/BlendState.h"
#include <Rendering/Renderers/TextureRenderer.h>

namespace MyEngine
{
	namespace Io
	{
		namespace Ttf
		{
			class FontRasterizer;
		}
	}

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

			void AddCharacterXy(const Math::Float3& offset, const Math::Float2& size, const Io::Ttf::FontRasterizer& rasterizer) const;
			void Render() const;

		private:
			TextureRenderer<V_PosUv, CB_CamMat>* m_pTextureRenderer;
			Gpu& m_Gpu;
		};
	}
}
