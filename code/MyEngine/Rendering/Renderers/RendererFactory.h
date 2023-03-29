#pragma once
#include "BasicRenderer.h"
#include "ConstantBufferTypes.h"
#include "TextureRenderer.h"
#include "VertexTypes.h"

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
			using WireframeRenderer = BasicRenderer<V_PosCol, CB_CamMat>;
			using SimpleTexRenderer = TextureRenderer<V_PosNorUv, CB_CamMatPos>;

			static SimpleRenderer* CreateSimpleRenderer(Gpu& gpu, Game::Camera& camera, bool wireframe = false);
			static WireframeRenderer* CreateWireframeRenderer(Gpu& gpu, Game::Camera& camera);
			static SimpleTexRenderer* CreateSimpleTexRenderer(Gpu& gpu, Game::Camera& camera);

			template<typename T>
			static T* Create(Gpu& gpu, Game::Camera& camera);
		};

		template <>
		inline RendererFactory::SimpleRenderer* RendererFactory::Create(Gpu& gpu, Game::Camera& camera)
		{
			return CreateSimpleRenderer(gpu, camera);
		}

		template <>
		inline RendererFactory::WireframeRenderer* RendererFactory::Create(Gpu& gpu, Game::Camera& camera)
		{
			return CreateWireframeRenderer(gpu, camera);
		}

		template <>
		inline RendererFactory::SimpleTexRenderer* RendererFactory::Create(Gpu& gpu, Game::Camera& camera)
		{
			return CreateSimpleTexRenderer(gpu, camera);
		}
	}
}
