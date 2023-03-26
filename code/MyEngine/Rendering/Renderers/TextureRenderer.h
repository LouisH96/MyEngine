#pragma once
#include "App/Wrappers/Dx/BlendState.h"
#include "App/Wrappers/Dx/ConstantBuffer.h"
#include "Math/Float2.h"
#include "Math/Float3.h"
#include "App/Wrappers/Dx/InputLayout.h"
#include "App/Wrappers/Dx/Mesh.h"
#include "App/Wrappers/Dx/RasterizerState.h"
#include <App/Wrappers/Dx/SamplerState.h>
#include <App/Wrappers/Dx/Texture.h>

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
		//---| Class |---
		template<typename Vertex, typename CamData>
		class TextureRenderer
		{
		public:
			using VertexType = Vertex;
			using CamDataRefType = CamData;

			//---| Construction |---
			TextureRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera, const std::wstring& shaderPath);
			~TextureRenderer();

			//---| Rule of Five |---
			TextureRenderer(const TextureRenderer& other) = delete;
			TextureRenderer(TextureRenderer&& other) noexcept = delete;
			TextureRenderer& operator=(const TextureRenderer& other) = delete;
			TextureRenderer& operator=(TextureRenderer&& other) noexcept = delete;

			//---| Loop |---
			void Render();

			//---| Operations |---
			void AddMesh(const Array<Vertex>& vertices, const Array<int>& indices, const std::wstring& texturePath);

		private:
			//---| Types |---
			struct MeshData
			{
				App::Wrappers::Dx::Mesh* pMesh;
				Dx::Texture* pTexture;
			};

			//---| General |---
			App::Wrappers::Dx::Gpu& m_Gpu;
			Game::Camera::Camera& m_Camera;
			Dx::BlendState m_BlendState;
			Dx::RasterizerState m_RasterizerState;
			Dx::SamplerState m_Sampler;

			//---| Mesh/Shader Specific |---
			static const Dx::InputLayout::Element ELEMENTS[];
			App::Wrappers::Dx::Shader m_Shader;
			Dx::InputLayout m_InputLayout;
			Dx::ConstantBuffer<CamData> m_ConstantBuffer;
			Array<MeshData> m_Meshes{};
		};

		template <typename Vertex, typename CamData>
		TextureRenderer<Vertex, CamData>::TextureRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera,
			const std::wstring& shaderPath)
			: m_Gpu(gpu)
			, m_Camera(camera)
			, m_BlendState(gpu)
			, m_RasterizerState(gpu)
			, m_Shader(gpu, shaderPath)
			, m_InputLayout(gpu, Vertex::ELEMENTS, Vertex::NR_ELEMENTS)
			, m_ConstantBuffer(gpu)
			, m_Sampler(gpu)
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
		void TextureRenderer<Vertex, CamData>::Render()
		{
			m_Sampler.ActivatePs(m_Gpu);
			m_ConstantBuffer.Update(m_Gpu, { m_Camera });
			m_ConstantBuffer.Activate(m_Gpu);
			m_RasterizerState.Activate(m_Gpu);
			m_InputLayout.Activate(m_Gpu);
			m_BlendState.Activate(m_Gpu);
			m_Shader.Activate();
			for (int i = 0; i < m_Meshes.GetSize(); i++)
			{
				m_Meshes[i].pTexture->ActivatePs(m_Gpu);
				m_Meshes[i].pMesh->Activate();
				m_Meshes[i].pMesh->DrawNotIndexed();
			}
		}

		template <typename Vertex, typename CamData>
		void TextureRenderer<Vertex, CamData>::AddMesh(const Array<Vertex>& vertices, const Array<int>& indices, const std::wstring& texturePath)
		{
			using namespace App::Wrappers::Dx;
			using namespace MyEngine::Dx;
			Mesh* pMesh = Mesh::Create<Vertex>(m_Gpu, vertices, indices);
			Texture* pTexture = new Texture(m_Gpu, texturePath);
			m_Meshes.Add({pMesh, pTexture});
		}
	}
}
