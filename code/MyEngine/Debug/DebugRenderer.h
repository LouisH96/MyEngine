#pragma once
#include <Rendering/State/BlendState.h>
#include <Rendering/State/ConstantBuffer.h>
#include <Rendering/State/InputLayout.h>
#include <Rendering/State/RasterizerState.h>
#include <Rendering/State/Shader.h>
#include <Rendering/Renderers/ConstantBufferTypes.h>
#include <Rendering/Renderers/VertexTypes.h>

namespace MyEngine
{
	//---| Forward Includes |---
	namespace Game
	{
		class Camera;
	}
	namespace Rendering
	{
		class Mesh;
		class Gpu;
	}

	//---| Class |---
	namespace Debug
	{

		class DebugRenderer
		{
		public:
			static void Init(Rendering::Gpu& gpu, Game::Camera& camera);
			static void Release();
			static void Render();

			static void AddSphere(const Math::Float3& position, const Math::Float3& color, float radius);
			static void AddSpheres(const Array<Math::Float3>& positions, const Math::Float3& color, float radius);

		private:
			using Vertex = Rendering::V_PosColNorm;

			//---| General |---
			static DebugRenderer* m_pStatic;
			Rendering::Gpu& m_Gpu;
			Game::Camera& m_Camera;

			//---| RenderState |---
			Rendering::BlendState m_BlendState;
			Rendering::RasterizerState m_RasterizerState;
			Rendering::InputLayout m_InputLayout;
			Rendering::ConstantBuffer<Rendering::CB_CamMatPos> m_ConstantBuffer;
			Rendering::Shader m_Shader;
			static const Rendering::InputLayout::Element ELEMENTS[];

			//---| Mesh |---
			Array<Rendering::Mesh*> m_Meshes{};

			//---| Init |---
			DebugRenderer(Rendering::Gpu& gpu, Game::Camera& camera);
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
