#pragma once
#include <Rendering/State/BlendState.h>
#include <Rendering/State/ConstantBuffer.h>
#include <Rendering/State/InputLayout.h>
#include <Rendering/State/Mesh.h>
#include <Rendering/State/RasterizerState.h>

namespace MyEngine
{
	namespace Game
	{
		class Camera;
	}
}

namespace MyEngine
{
	namespace Rendering
	{
		template<typename Vertex, typename CamData>
		class BasicRenderer
		{
		public:
			using VertexType = Vertex;
			using CamDataRefType = CamData;

			//---| Construction |---
			BasicRenderer(Gpu& gpu, Game::Camera& camera, const std::wstring& shaderPath, bool isWireframe = false);
			~BasicRenderer();

			//---| Rule of Five |---
			BasicRenderer(const BasicRenderer& other) = delete;
			BasicRenderer(BasicRenderer&& other) noexcept = delete;
			BasicRenderer& operator=(const BasicRenderer& other) = delete;
			BasicRenderer& operator=(BasicRenderer&& other) noexcept = delete;

			//---| Loop |---
			void Render();

			//---| Operations |---
			void AddMesh(const Array<Vertex>& vertices, const Array<int>& indices);
			void AddMesh(Mesh* pMesh);

		private:
			//---| General |---
			Gpu& m_Gpu;
			Game::Camera& m_Camera;
			BlendState m_BlendState;
			RasterizerState m_RasterizerState;

			//---| Mesh/Shader Specific |---
			static const InputLayout::Element ELEMENTS[];
			Shader m_Shader;
			InputLayout m_InputLayout;
			ConstantBuffer<CamData> m_ConstantBuffer;
			Array<Mesh*> m_Meshes{};
		};

		template <typename Vertex, typename CamData>
		BasicRenderer<Vertex, CamData>::BasicRenderer(Gpu& gpu, Game::Camera& camera,
			const std::wstring& shaderPath, bool isWireframe)
			: m_Gpu(gpu)
			, m_Camera(camera)
			, m_BlendState(gpu)
			, m_RasterizerState(gpu, isWireframe)
			, m_Shader(gpu, shaderPath)
			, m_InputLayout(gpu, Vertex::ELEMENTS, Vertex::NR_ELEMENTS)
			, m_ConstantBuffer(gpu)
		{
		}

		template <typename Vertex, typename CamData>
		BasicRenderer<Vertex, CamData>::~BasicRenderer()
		{
			m_Meshes.DeleteAll();
		}

		template <typename Vertex, typename CamData>
		void BasicRenderer<Vertex, CamData>::Render()
		{
			m_ConstantBuffer.Update(m_Gpu, CamData{ m_Camera });
			m_ConstantBuffer.Activate(m_Gpu);
			m_RasterizerState.Activate(m_Gpu);
			m_InputLayout.Activate(m_Gpu);
			m_BlendState.Activate(m_Gpu);
			m_Shader.Activate();
			for (int i = 0; i < m_Meshes.GetSize(); i++)
			{
				m_Meshes[i]->Activate(m_Gpu);
				m_Meshes[i]->Draw(m_Gpu);
			}
		}

		template <typename Vertex, typename CamData>
		void BasicRenderer<Vertex, CamData>::AddMesh(const Array<Vertex>& vertices, const Array<int>& indices)
		{
			m_Meshes.Add(Mesh::Create<Vertex>(m_Gpu, vertices, indices));
		}

		template <typename Vertex, typename CamData>
		void BasicRenderer<Vertex, CamData>::AddMesh(Mesh* pMesh)
		{
			m_Meshes.Add(pMesh);
		}
	}
}

