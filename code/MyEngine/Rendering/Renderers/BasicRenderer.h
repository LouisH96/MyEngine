#pragma once
#include <Rendering/State/BlendState.h>
#include <Rendering/State/ConstantBuffer.h>
#include <Rendering/State/InputLayout.h>
#include <Rendering/State/Mesh.h>
#include <Rendering/State/Shader.h>
#include <Rendering/State/RasterizerState.h>

#include "Game/Camera/Camera.h"
#include "Rendering/State/DepthStencilState.h"

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
			BasicRenderer(const std::wstring& shaderPath, bool isWireframe = false);
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
			BlendState m_BlendState;
			RasterizerState m_RasterizerState;
			DepthStencilState m_DepthStencilState;

			//---| Mesh/Shader Specific |---
			static const InputLayout::Element ELEMENTS[];
			Shader m_Shader;
			InputLayout m_InputLayout;
			ConstantBuffer<CamData> m_ConstantBuffer;
			Array<Mesh*> m_Meshes{};
		};

		template <typename Vertex, typename CamData>
		BasicRenderer<Vertex, CamData>::BasicRenderer(
			const std::wstring& shaderPath, bool isWireframe)
			: m_RasterizerState(isWireframe)
			, m_Shader(shaderPath)
			, m_InputLayout(Vertex::ELEMENTS, Vertex::NR_ELEMENTS)
			, m_ConstantBuffer{}
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
			m_DepthStencilState.Activate();
			m_ConstantBuffer.Update(CamData{ Globals::pCamera->GetPosition(), Globals::pCamera->GetViewProjection()});
			m_ConstantBuffer.Activate();
			m_RasterizerState.Activate();
			m_InputLayout.Activate();
			m_BlendState.Activate();
			m_Shader.Activate();
			for (int i = 0; i < m_Meshes.GetSize(); i++)
			{
				m_Meshes[i]->Activate();
				m_Meshes[i]->Draw();
			}
		}

		template <typename Vertex, typename CamData>
		void BasicRenderer<Vertex, CamData>::AddMesh(const Array<Vertex>& vertices, const Array<int>& indices)
		{
			m_Meshes.Add(Mesh::Create<Vertex>(vertices, indices));
		}

		template <typename Vertex, typename CamData>
		void BasicRenderer<Vertex, CamData>::AddMesh(Mesh* pMesh)
		{
			m_Meshes.Add(pMesh);
		}
	}
}

