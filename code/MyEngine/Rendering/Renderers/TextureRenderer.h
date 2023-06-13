#pragma once
#include "Rendering/State/BlendState.h"
#include "Rendering/State/ConstantBuffer.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/Mesh.h"
#include "Rendering/State/RasterizerState.h"
#include <Rendering/State/SamplerState.h>
#include <Rendering/State/Texture.h>

namespace MyEngine
{
	namespace Game
	{
		class FocusPointCamera;
	}

	namespace Rendering
	{
		//---| Class |---
		template<typename Vertex, typename CamData>
		class TextureRenderer
		{
		public:
			using VertexType = Vertex;
			using CamDataRefType = CamData;

			//---| Construction |---
			TextureRenderer(Gpu& gpu, const std::wstring& shaderPath);
			explicit TextureRenderer(const std::wstring& shaderPath);
			~TextureRenderer();

			//---| Rule of Five |---
			TextureRenderer(const TextureRenderer& other) = delete;
			TextureRenderer(TextureRenderer&& other) noexcept = delete;
			TextureRenderer& operator=(const TextureRenderer& other) = delete;
			TextureRenderer& operator=(TextureRenderer&& other) noexcept = delete;

			//---| Loop |---
			void Render(const Math::Float3& cameraPosition, const Math::Float4X4& viewProjection);

			//---| Operations |---
			void AddMesh(const Array<Vertex>& vertices, const Array<int>& indices, const std::wstring& texturePath);
			void AddMesh(const Array<Vertex>& vertices, const std::wstring& texturePath);
			void AddMesh(Mesh* pMesh, Texture* pTexture);

		private:
			//---| Types |---
			struct MeshData
			{
				Mesh* pMesh;
				Texture* pTexture;
			};

			//---| General |---
			Gpu& m_Gpu;
			BlendState m_BlendState;
			RasterizerState m_RasterizerState;
			SamplerState m_Sampler;
			DepthStencilState m_DepthStencilState;

			//---| Mesh/Shader Specific |---
			static const InputLayout::Element ELEMENTS[];
			Shader m_Shader;
			InputLayout m_InputLayout;
			ConstantBuffer<CamData> m_ConstantBuffer;
			Array<MeshData> m_Meshes{};
		};

		template <typename Vertex, typename CamData>
		TextureRenderer<Vertex, CamData>::TextureRenderer(Gpu& gpu, const std::wstring& shaderPath)
			: m_Gpu(gpu)
			, m_RasterizerState(gpu)
			, m_Sampler(gpu)
			, m_Shader(gpu, shaderPath)
			, m_InputLayout(Vertex::ELEMENTS, Vertex::NR_ELEMENTS)
		{
		}

		template <typename Vertex, typename CamData>
		TextureRenderer<Vertex, CamData>::TextureRenderer(const std::wstring& shaderPath)
			: TextureRenderer{ *Globals::pGpu, shaderPath }
		{
		}

		template <typename Vertex, typename CamData>
		TextureRenderer<Vertex, CamData>::~TextureRenderer()
		{
			for (int i = 0; i < m_Meshes.GetSize(); i++)
			{
				delete m_Meshes[i].pMesh;
				delete m_Meshes[i].pTexture;
			}
		}

		template <typename Vertex, typename CamData>
		void TextureRenderer<Vertex, CamData>::Render(const Math::Float3& cameraPosition,
			const Math::Float4X4& viewProjection)
		{
			m_DepthStencilState.Activate();
			m_Sampler.ActivatePs(m_Gpu);
			m_ConstantBuffer.Update(m_Gpu, CamData{ cameraPosition, viewProjection });
			m_ConstantBuffer.Activate(m_Gpu);
			m_RasterizerState.Activate(m_Gpu);
			m_InputLayout.Activate(m_Gpu);
			m_BlendState.Activate(m_Gpu);
			m_Shader.Activate();
			for (int i = 0; i < m_Meshes.GetSize(); i++)
			{
				m_Meshes[i].pTexture->ActivatePs(m_Gpu);
				m_Meshes[i].pMesh->Activate(m_Gpu);
				m_Meshes[i].pMesh->Draw(m_Gpu);
			}
		}

		template <typename Vertex, typename CamData>
		void TextureRenderer<Vertex, CamData>::AddMesh(const Array<Vertex>& vertices, const Array<int>& indices, const std::wstring& texturePath)
		{
			Mesh* pMesh = Mesh::Create<Vertex>(m_Gpu, vertices, indices);
			Texture* pTexture = new Texture(m_Gpu, texturePath);
			m_Meshes.Add({ pMesh, pTexture });
		}

		template <typename Vertex, typename CamData>
		void TextureRenderer<Vertex, CamData>::AddMesh(const Array<Vertex>& vertices, const std::wstring& texturePath)
		{
			Mesh* pMesh = Mesh::Create<Vertex>(m_Gpu, vertices);
			Texture* pTexture = new Texture(m_Gpu, texturePath);
			m_Meshes.Add({ pMesh, pTexture });
		}

		template <typename Vertex, typename CamData>
		void TextureRenderer<Vertex, CamData>::AddMesh(Mesh* pMesh, Texture* pTexture)
		{
			m_Meshes.Add({ pMesh, pTexture });
		}
	}
}
