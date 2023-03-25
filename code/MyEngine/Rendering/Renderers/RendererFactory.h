#pragma once
#include "BasicRenderer.h"
#include "ConstantBufferTypes.h"
#include "TextureRenderer.h"

namespace MyEngine
{
	namespace App
	{
		namespace Wrappers
		{
			 namespace Dx
			 {
				 class Gpu;
			 }
		}
	}
	namespace Game
	{
		namespace Camera
		{
			class Camera;
		}
	}

	namespace Rendering
	{
		class RendererFactory
		{
		public:
			using SimpleRenderer = BasicRenderer<V_PosColNorm, CB_CamMatPos>;
			using WireframeRenderer = BasicRenderer<V_PosCol, CB_CamMat>;
			using SimpleTexRenderer = TextureRenderer<V_PosNorUv, CB_CamMatPos>;

			static SimpleRenderer* CreateSimpleRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera);
			static WireframeRenderer* CreateWireframeRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera);
			static SimpleTexRenderer* CreateSimpleTexRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera);

			template<typename T>
			static T* Create(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera);
		};

		template <>
		inline RendererFactory::SimpleRenderer* RendererFactory::Create(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera)
		{
			return CreateSimpleRenderer(gpu, camera);
		}

		template <>
		inline RendererFactory::WireframeRenderer* RendererFactory::Create(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera)
		{
			return CreateWireframeRenderer(gpu, camera);
		}

		template <>
		inline RendererFactory::SimpleTexRenderer* RendererFactory::Create(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera)
		{
			return CreateSimpleTexRenderer(gpu, camera);
		}
	}
}
