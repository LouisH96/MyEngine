#pragma once
#include "BasicRenderer.h"
	#include <Rendering/Structs/VertexTypes.h>
	#include <Rendering/Structs/ConstantBufferTypes.h>
#include "TextureRenderer.h"

namespace MyEngine
{
	namespace Game
	{
		class Camera;
	}

	namespace Rendering
	{
		class Gpu;

		class RendererFactory
		{
		public:
			using SimpleRenderer = BasicRenderer<V_PosColNorm, CB_CamMatPos>;
			using UnlitRenderer = BasicRenderer<V_PosCol, CB_CamMat>;
			using SimpleTexRenderer = TextureRenderer<V_PosNorUv, CB_CamMatPos>;

			static SimpleRenderer* CreateSimpleRenderer(Gpu& gpu, bool wireframe = false);
			static UnlitRenderer* CreateUnlitRenderer(Gpu& gpu, bool wireframe = true);
			static SimpleTexRenderer* CreateSimpleTexRenderer(Gpu& gpu);

			template<typename T>
			static T* Create(Gpu& gpu);
		};

		template <>
		inline RendererFactory::SimpleRenderer* RendererFactory::Create(Gpu& gpu)
		{
			return CreateSimpleRenderer(gpu);
		}

		template <>
		inline RendererFactory::UnlitRenderer* RendererFactory::Create(Gpu& gpu)
		{
			return CreateUnlitRenderer(gpu);
		}

		template <>
		inline RendererFactory::SimpleTexRenderer* RendererFactory::Create(Gpu& gpu)
		{
			return CreateSimpleTexRenderer(gpu);
		}
	}
}
