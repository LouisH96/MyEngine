#pragma once
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
		class BasicRenderer
		{
		public:
			//---| Types |---
			struct MeshVertex
			{
				Math::Float3 Pos{};
				Math::Float3 Color{};
				Math::Float3 Normal{};
			};

			//---| Construction |---
			BasicRenderer(App::Wrappers::Dx::Gpu& gpu, Game::Camera::Camera& camera);
			~BasicRenderer();

			//---| Rule of Five |---
			BasicRenderer(const BasicRenderer& other) = delete;
			BasicRenderer(BasicRenderer&& other) noexcept = delete;
			BasicRenderer& operator=(const BasicRenderer& other) = delete;
			BasicRenderer& operator=(BasicRenderer&& other) noexcept = delete;

			//---| Loop |---
			void Render();

			//---| Operations |---
			void AddMesh(const Array<Math::Float3>& points, const Array<Math::Float3>& normals, const Math::Float3& color, const Array<int>& indices);
			void AddMesh(const Array<MeshVertex>& vertices, const Array<int>& indices);

		private:
			//---| Types |---
			struct CBuffer
			{
				DirectX::XMFLOAT4X4 CameraMatrix;
				Math::Float3 CameraPos;
				float padding;
			};

			//---| General |---
			App::Wrappers::Dx::Gpu& m_Gpu;
			Game::Camera::Camera& m_Camera;
			Dx::BlendState m_BlendState;
			Dx::RasterizerState m_RasterizerState;

			//---| Mesh/Shader Specific |---
			static const Dx::InputLayout::Element ELEMENTS[];
			App::Wrappers::Dx::Shader m_Shader;
			Dx::InputLayout m_InputLayout;
			Dx::ConstantBuffer<CBuffer> m_ConstantBuffer;
			Array<App::Wrappers::Dx::Mesh*> m_Meshes{};
		};
	}
}

