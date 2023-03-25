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
		//---| Types |---
		struct V_PosUv
		{
			Math::Float3 Pos{};
			Math::Float2 Uv{};

			static const Dx::InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosNorUv
		{
			Math::Float3 Pos{};
			Math::Float3 Normal{};
			Math::Float2 Uv{};

			static const Dx::InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 3;
		};

		//---| Class |---
		template<typename Vertex, typename CamData>
		class TextureRenderer
		{
		public:
			using VertexType = Vertex;
			using CamDataRefType = CamData;

			//---| Construction |---
			TextureRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera, const std::wstring& shaderPath, const std::wstring& texturePath);
			~TextureRenderer();

			//---| Rule of Five |---
			TextureRenderer(const TextureRenderer& other) = delete;
			TextureRenderer(TextureRenderer&& other) noexcept = delete;
			TextureRenderer& operator=(const TextureRenderer& other) = delete;
			TextureRenderer& operator=(TextureRenderer&& other) noexcept = delete;

			//---| Loop |---
			void Render();

			//---| Operations |---
			void AddMesh(const Array<Vertex>& vertices, const Array<int>& indices);

		private:
			//---| General |---
			App::Wrappers::Dx::Gpu& m_Gpu;
			Game::Camera::Camera& m_Camera;
			Dx::BlendState m_BlendState;
			Dx::RasterizerState m_RasterizerState;
			Dx::SamplerState m_Sampler;
			Dx::Texture m_Texture;

			//---| Mesh/Shader Specific |---
			static const Dx::InputLayout::Element ELEMENTS[];
			App::Wrappers::Dx::Shader m_Shader;
			Dx::InputLayout m_InputLayout;
			Dx::ConstantBuffer<CamData> m_ConstantBuffer;
			Array<App::Wrappers::Dx::Mesh*> m_Meshes{};
		};

		template <typename Vertex, typename CamData>
		TextureRenderer<Vertex, CamData>::TextureRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera,
			const std::wstring& shaderPath, const std::wstring& texturePath)
			: m_Gpu(gpu)
			, m_Camera(camera)
			, m_BlendState(gpu)
			, m_RasterizerState(gpu)
			, m_Shader(gpu, shaderPath)
			, m_InputLayout(gpu, Vertex::ELEMENTS, Vertex::NR_ELEMENTS)
			, m_ConstantBuffer(gpu)
			, m_Sampler(gpu)
			, m_Texture(gpu, texturePath)
		{
		}

		template <typename Vertex, typename CamData>
		TextureRenderer<Vertex, CamData>::~TextureRenderer()
		{
			m_Meshes.DeleteAll();
		}

		template <typename Vertex, typename CamData>
		void TextureRenderer<Vertex, CamData>::Render()
		{
			m_Sampler.ActivatePs(m_Gpu);
			m_Texture.ActivatePs(m_Gpu);
			m_ConstantBuffer.Update(m_Gpu, { m_Camera });
			m_ConstantBuffer.Activate(m_Gpu);
			m_RasterizerState.Activate(m_Gpu);
			m_InputLayout.Activate(m_Gpu);
			m_BlendState.Activate(m_Gpu);
			m_Shader.Activate();
			for (int i = 0; i < m_Meshes.GetSize(); i++)
			{
				m_Meshes[i]->Activate();
				m_Meshes[i]->DrawNotIndexed();
			}
		}

		template <typename Vertex, typename CamData>
		void TextureRenderer<Vertex, CamData>::AddMesh(const Array<Vertex>& vertices, const Array<int>& indices)
		{
			m_Meshes.Add(App::Wrappers::Dx::Mesh::Create<Vertex>(m_Gpu, vertices, indices));
		}
	}
}
