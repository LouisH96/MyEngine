#pragma once
#include <Rendering/Structs/VertexTypes.h>
#include <Rendering/Structs/ConstantBufferTypes.h>

#include "Rendering/State/ConstantBuffer.h"
#include "Rendering/State/Shader.h"
#include <Rendering\State\RasterizerState.h>
#include <Rendering\State\BlendState.h>
#include <Rendering\State\SamplerState.h>

namespace MyEngine
{
	namespace Game
	{
		class Transform;
		class Camera;
	}

	namespace Rendering
	{
		class Mesh;
		class Texture;

		class R_LambertCam_Tex_Transform
		{
		public:
			using Vertex = V_PosNorUv;

			R_LambertCam_Tex_Transform(Gpu& gpu, Game::Camera& camera);
			~R_LambertCam_Tex_Transform() = default;
			R_LambertCam_Tex_Transform(const R_LambertCam_Tex_Transform& other) = delete;
			R_LambertCam_Tex_Transform(R_LambertCam_Tex_Transform&& other) noexcept = delete;
			R_LambertCam_Tex_Transform& operator=(const R_LambertCam_Tex_Transform& other) = delete;
			R_LambertCam_Tex_Transform& operator=(R_LambertCam_Tex_Transform&& other) noexcept = delete;

			//---| Loop |---
			void Render();

			//---| Operations |---
			void AddEntry(Mesh& mesh, Texture& texture, Game::Transform& transform);

		private:
			//---| Types |---
			struct DrawEntry
			{
				Mesh* pMesh;
				Texture* pTexture;
				Game::Transform* pTransform;
			};

			Gpu& m_Gpu;
			Game::Camera& m_Camera;

			BlendState m_BlendState;
			RasterizerState m_RasterizerState;
			SamplerState m_Sampler;
			InputLayout m_InputLayout;
			Shader m_Shader;

			ConstantBuffer<CB_CamMatPos> m_ConstantBuffer;
			Array<DrawEntry> m_Entries;
		};
	}
}
