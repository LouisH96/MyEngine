#pragma once
#include <Rendering/Structs/ConstantBufferTypes.h>
#include <Rendering/Structs/VertexTypes.h>
#include <Rendering/Renderers/BasicRenderer.h>
#include <Rendering/Renderers/TextureRenderer.h>

namespace MyEngine
{
	class Camera;

	namespace Rendering
	{
		class RendererFactory
		{
		public:
			using SimpleRenderer = BasicRenderer<V_PosColNorm, CB_CamMatPos>;
			using UnlitRenderer = BasicRenderer<V_PosCol, CB_CamMat>;
			using SimpleTexRenderer = TextureRenderer<V_PosNorUv, CB_CamMatPos>;

			static SimpleRenderer* CreateSimpleRenderer(bool wireframe = false);
			static UnlitRenderer* CreateUnlitRenderer(bool wireframe = true);
			static SimpleTexRenderer* CreateSimpleTexRenderer();

			template<typename T>
			static T* Create();
		};

		template <>
		inline RendererFactory::SimpleRenderer* RendererFactory::Create()
		{
			return CreateSimpleRenderer();
		}

		template <>
		inline RendererFactory::UnlitRenderer* RendererFactory::Create()
		{
			return CreateUnlitRenderer();
		}

		template <>
		inline RendererFactory::SimpleTexRenderer* RendererFactory::Create()
		{
			return CreateSimpleTexRenderer();
		}
	}
}
