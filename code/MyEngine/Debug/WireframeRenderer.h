#pragma once
#include "App/Wrappers/Dx/Mesh.h"
#include "Math/Float3.h"

namespace MyEngine
{
	//---| Forward Declarations |---
	namespace Game
	{
		namespace Camera
		{
			class Camera;
		}
	}
	namespace App
	{
		namespace Wrappers
		{
			namespace Dx
			{
				class Gpu;
				class Shader;
			}
		}
	}
	namespace Dx
	{
		class BlendState;
		class RasterizerState;
		class InputLayout;
		template<typename T> class ConstantBuffer;
	} 

	//---| Class |---
	namespace Debug
	{
		class WireframeRenderer
		{
		public:
			//---| Types |---
			struct WireframeVertex
			{
				Math::Float3 Position{};
				Math::Float3 Color{};
			};
			using MeshId = int;

			//---| Init |---
			WireframeRenderer(const App::Wrappers::Dx::Gpu& gpu);
			~WireframeRenderer();

			WireframeRenderer(const WireframeRenderer& other) = delete;
			WireframeRenderer(WireframeRenderer&& other) noexcept = delete;
			WireframeRenderer& operator=(const WireframeRenderer& other) = delete;
			WireframeRenderer& operator=(WireframeRenderer&& other) noexcept = delete;

			//---| Operations |---
			void AddMesh(const App::Wrappers::Dx::Gpu& gpu, const Array<Math::Float3>& points, const Math::Float3& color, const Array<int>& indices);
			void AddMesh(const App::Wrappers::Dx::Gpu& gpu, const Array<WireframeVertex>& vertices, const Array<int>& indices);

			void Update(const App::Wrappers::Dx::Gpu& gpu, const Game::Camera::Camera& camera) const;
			void Draw(const App::Wrappers::Dx::Gpu& gpu);

		private:
			//---| Types |---
			struct ConstantBuffer
			{
				DirectX::XMFLOAT4X4 cameraMatrix;
			};
			//---| Fields |---
			Dx::BlendState* m_pBlendState{};
			Dx::RasterizerState* m_pRasterizerState{};
			Dx::InputLayout* m_pInputLayout{};
			App::Wrappers::Dx::Shader* m_pShader{};
			Dx::ConstantBuffer<ConstantBuffer>* m_pConstantBuffer{};
			Array<App::Wrappers::Dx::Mesh*> m_Meshes{};
			//---| Operations |---
			void InitRenderStates(const App::Wrappers::Dx::Gpu& gpu);
		};
	}
}
