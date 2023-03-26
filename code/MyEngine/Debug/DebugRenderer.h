#pragma once
#include "App/Wrappers/Dx/BlendState.h"
#include "App/Wrappers/Dx/ConstantBuffer.h"
#include "App/Wrappers/Dx/InputLayout.h"
#include "App/Wrappers/Dx/RasterizerState.h"
#include "App/Wrappers/Dx/Shader.h"
#include <Rendering/Renderers/ConstantBufferTypes.h>
#include <Rendering/Renderers/VertexTypes.h>

namespace MyEngine
{
	//---| Forward Includes |---
	namespace App
	{
		namespace Wrappers
		{
			namespace Dx
			{
				class Mesh;
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

	//---| Class |---
	namespace Debug
	{
		class DebugRenderer
		{
		public:
			static void Init(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera);
			static void Release();
			static void Render();

			static void AddSphere(const Math::Float3& position, const Math::Float3& color, float radius);
			static void AddSpheres(const Array<Math::Float3>& positions, const Math::Float3& color, float radius);

		private:
			using Vertex = Rendering::V_PosColNorm;

			//---| General |---
			static DebugRenderer* m_pStatic;
			App::Wrappers::Dx::Gpu& m_Gpu;
			Game::Camera::Camera& m_Camera;

			//---| RenderState |---
			Dx::BlendState m_BlendState;
			Dx::RasterizerState m_RasterizerState;
			Dx::InputLayout m_InputLayout;
			Dx::ConstantBuffer<Rendering::CB_CamMatPos> m_ConstantBuffer;
			App::Wrappers::Dx::Shader m_Shader;
			static const Dx::InputLayout::Element ELEMENTS[];

			//---| Mesh |---
			Array<App::Wrappers::Dx::Mesh*> m_Meshes{};

			//---| Init |---
			DebugRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera);
			~DebugRenderer();

			//---| Rule of Five |---
			DebugRenderer(const DebugRenderer& other) = delete;
			DebugRenderer(DebugRenderer&& other) noexcept = delete;
			DebugRenderer& operator=(const DebugRenderer& other) = delete;
			DebugRenderer& operator=(DebugRenderer&& other) noexcept = delete;

			//---| Loop |---
			void Class_Render();

			//---| Operations |---
			void Class_AddSphere(const Math::Float3& position, const Math::Float3& color, float radius);
			void Class_AddSpheres(const Array<Math::Float3>& positions, const Math::Float3& color, float radius);
		};
	}
}
