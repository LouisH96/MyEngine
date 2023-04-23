#pragma once
#include <Rendering/State/BlendState.h>
#include <Rendering/State/ConstantBuffer.h>
#include <Rendering/State/InputLayout.h>
#include <Rendering/State/RasterizerState.h>
#include <Rendering/State/Shader.h>
#include <Rendering/Structs/ConstantBufferTypes.h>
#include <Rendering/Structs/VertexTypes.h>
#include <Rendering/Renderers/RendererFactory.h>

namespace MyEngine
{
	//---| Forward Includes |---
	namespace Game
	{
		class FocusPointCamera;
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
			//---| Rule of Five |---
			DebugRenderer(const DebugRenderer& other) = delete;
			DebugRenderer(DebugRenderer&& other) noexcept = delete;
			DebugRenderer& operator=(const DebugRenderer& other) = delete;
			DebugRenderer& operator=(DebugRenderer&& other) noexcept = delete;

			static void Init(Rendering::Gpu& gpu, Game::FocusPointCamera& camera);
			static void Release();
			static void Render();

			static void AddSphere(const Math::Float3& position, const Math::Float3& color, float radius);
			static void AddSpheres(const Array<Math::Float3>& positions, const Math::Float3& color, float radius);

			static void AddLine(const Math::Float3& begin, const Math::Float3& end, const Math::Float3& color);
			static void AddLine(const Array<Math::Float3>& points, const Math::Float3& color);

			static void AddRect(const Math::Float3& leftTop, const Math::Float3& rightTop, const Math::Float3& rightBot, const Math::Float3& leftBot, const Math::Float3& color);;
			template<typename T> static void AddRect(const Math::Value2<T>& min, const Math::Value2<T>& max, const Math::Float3& offset, const Math::Float3& color);

			static void AddGridXy(const Math::Float3& offset, const Math::Float2& bounds, int nrCols, int nrRows, const Math::Float3& color);

		private:
			using Vertex = Rendering::V_PosColNorm;
			using LineRenderer = Rendering::RendererFactory::UnlitRenderer;
			using LineVertex = LineRenderer::VertexType;

			//---| General |---
			static DebugRenderer* m_pStatic;
			Rendering::Gpu& m_Gpu;
			Game::FocusPointCamera& m_Camera;

			//---| RenderState |---
			Rendering::BlendState m_BlendState;
			Rendering::RasterizerState m_RasterizerState;
			Rendering::InputLayout m_InputLayout;
			Rendering::ConstantBuffer<Rendering::CB_CamMatPos> m_ConstantBuffer;
			Rendering::Shader m_Shader;
			static const Rendering::InputLayout::Element ELEMENTS[];
			LineRenderer* m_pLineRenderer{};

			//---| Mesh |---
			Array<Rendering::Mesh*> m_Meshes{};

			//---| Init |---
			DebugRenderer(Rendering::Gpu& gpu, Game::FocusPointCamera& camera);
			~DebugRenderer();

			//---| Loop |---
			void Class_Render();

			//---| Operations |---
			void Class_AddSphere(const Math::Float3& position, const Math::Float3& color, float radius);
			void Class_AddSpheres(const Array<Math::Float3>& positions, const Math::Float3& color, float radius);
			void Class_AddLine(const Math::Float3& begin, const Math::Float3& end, const Math::Float3& color) const;
			void Class_AddLine(const Array<Math::Float3>& points, const Math::Float3& color) const;
			void Class_AddRect(const Math::Float3& leftTop, const Math::Float3& rightTop, const Math::Float3& rightBot, const Math::Float3& leftBot, const Math::Float3& color) const;
			template<typename T> void Class_AddRect(const Math::Value2<T>& min, const Math::Value2<T>& max, const Math::Float3& offset, const Math::Float3& color);
		};

		template <typename T>
		void DebugRenderer::AddRect(const Math::Value2<T>& min, const Math::Value2<T>& max, const Math::Float3& offset, const Math::Float3& color)
		{
			m_pStatic->Class_AddRect(min, max, offset, color);
		}

		template <typename T>
		void DebugRenderer::Class_AddRect(const Math::Value2<T>& min, const Math::Value2<T>& max, const Math::Float3& offset,
			const Math::Float3& color)
		{
			using namespace Math;
			const float left{ static_cast<float>(min.x) + offset.x };
			const float right{ static_cast<float>(max.x) + offset.x };
			const float bot{ static_cast<float>(min.y) + offset.y };
			const float top{ static_cast<float>(max.y) + offset.y };
			Array<LineVertex> vertices{ 5 };
			vertices[0] = { {left, bot, offset.z}, color };
			vertices[1] = { {left, top, offset.z}, color };
			vertices[2] = { {right, top, offset.z}, color };
			vertices[3] = { {right, bot, offset.z}, color };
			vertices[4] = vertices[0];
			m_pLineRenderer->AddMesh(Rendering::Mesh::Create(m_Gpu, vertices, Rendering::Topology::LineStrip));
		}
	}
}
