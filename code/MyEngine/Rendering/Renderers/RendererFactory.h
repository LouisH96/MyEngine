#pragma once
#include "BasicRenderer.h"
	#include <Rendering/Structs/VertexTypes.h>
	#include <Rendering/Structs/ConstantBufferTypes.h>
#include "TextureRenderer.h"

namespace MyEngine
{
	namespace Game
	{
		class FocusPointCamera;
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

			static SimpleRenderer* CreateSimpleRenderer(Gpu& gpu, Game::FocusPointCamera& camera, bool wireframe = false);
			static UnlitRenderer* CreateUnlitRenderer(Gpu& gpu, Game::FocusPointCamera& camera, bool wireframe = true);
			static SimpleTexRenderer* CreateSimpleTexRenderer(Gpu& gpu, Game::FocusPointCamera& camera);

			template<typename T>
			static T* Create(Gpu& gpu, Game::FocusPointCamera& camera);
		};

		template <>
		inline RendererFactory::SimpleRenderer* RendererFactory::Create(Gpu& gpu, Game::FocusPointCamera& camera)
		{
			return CreateSimpleRenderer(gpu, camera);
		}

		template <>
		inline RendererFactory::UnlitRenderer* RendererFactory::Create(Gpu& gpu, Game::FocusPointCamera& camera)
		{
			return CreateUnlitRenderer(gpu, camera);
		}

		template <>
		inline RendererFactory::SimpleTexRenderer* RendererFactory::Create(Gpu& gpu, Game::FocusPointCamera& camera)
		{
			return CreateSimpleTexRenderer(gpu, camera);
		}
	}
}
