#pragma once
#include "App/Resources.h"
#include "App/Wrappers/Dx/BlendState.h"
#include "App/Wrappers/Dx/ConstantBuffer.h"
#include "App/Wrappers/Dx/InputLayout.h"
#include "App/Wrappers/Dx/Mesh.h"
#include "App/Wrappers/Dx/RasterizerState.h"
#include "Math/Float3.h"

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
}

namespace MyEngine
{
	namespace Rendering
	{
		//---| Types |---
		struct V_PosCol
		{
			Math::Float3 Pos{};
			Math::Float3 Color{};
			
			static const Dx::InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 2;
		};
		struct V_PosColNorm
		{
			Math::Float3 Pos{};
			Math::Float3 Color{};
			Math::Float3 Normal{};

			static const Dx::InputLayout::Element ELEMENTS[];
			static constexpr int NR_ELEMENTS = 3;
		};
		struct CB_CamMat
		{
			DirectX::XMFLOAT4X4 CameraMatrix;

			CB_CamMat() = default;
			CB_CamMat(const Game::Camera::Camera& cam);
		};
		struct CB_CamMatPos
		{
			DirectX::XMFLOAT4X4 CameraMatrix;
			Math::Float3 CameraPos;
			float padding = 0;

			CB_CamMatPos() = default;
			CB_CamMatPos(const Game::Camera::Camera & cam);
		};

		template<typename Vertex, typename CamData>
		class BasicRenderer
		{
		public:
			using VertexType = Vertex;
			using CamDataRefType = CamData;

			//---| Construction |---
			BasicRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera, const std::wstring& shaderPath, bool isWireframe = false);
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

		private:
			//---| General |---
			App::Wrappers::Dx::Gpu& m_Gpu;
			Game::Camera::Camera& m_Camera;
			Dx::BlendState m_BlendState;
			Dx::RasterizerState m_RasterizerState;

			//---| Mesh/Shader Specific |---
			static const Dx::InputLayout::Element ELEMENTS[];
			App::Wrappers::Dx::Shader m_Shader;
			Dx::InputLayout m_InputLayout;
			Dx::ConstantBuffer<CamData> m_ConstantBuffer;
			Array<App::Wrappers::Dx::Mesh*> m_Meshes{};
		};

		template <typename Vertex, typename CamData>
		BasicRenderer<Vertex, CamData>::BasicRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera,
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
			m_ConstantBuffer.Update(m_Gpu, {m_Camera});
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
		void BasicRenderer<Vertex, CamData>::AddMesh(const Array<Vertex>& vertices, const Array<int>& indices)
		{
			m_Meshes.Add(App::Wrappers::Dx::Mesh::Create<Vertex>(m_Gpu, vertices, indices));
		}
	}
}

