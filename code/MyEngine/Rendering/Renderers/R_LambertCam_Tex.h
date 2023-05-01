#pragma once
#include "Math/Float4X4.h"
#include "Rendering/State/BlendState.h"
#include "Rendering/State/ConstantBuffer.h"
#include "Rendering/State/RasterizerState.h"
#include "Rendering/State/SamplerState.h"
#include "Rendering/State/Shader.h"
#include "Rendering/Structs/ConstantBufferTypes.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace Game
	{
		class Transform;
	}
}

namespace MyEngine
{
	namespace Rendering
	{
		class Mesh;
		class Texture;

		class R_LambertCam_Tex
		{
		public:
			//---| Type Alias |---
			using Vertex = V_PosNorUv;

			//---| Constructor/Destructor |---
			explicit R_LambertCam_Tex(Gpu& gpu);
			~R_LambertCam_Tex() = default;

			//---| Rule of five |---
			R_LambertCam_Tex(const R_LambertCam_Tex& other) = delete;
			R_LambertCam_Tex(R_LambertCam_Tex && other) noexcept = delete;
			R_LambertCam_Tex& operator=(const R_LambertCam_Tex & other) = delete;
			R_LambertCam_Tex& operator=(R_LambertCam_Tex && other) noexcept = delete;

			//---| Loop |---
			void Render(const Math::Float3& cameraPosition, const Math::Float4X4& viewProjection);

			//---| Operations |---
			void AddEntry(Mesh& mesh, Texture& texture);

		private:
			struct DrawEntry
			{
				Mesh* pMesh;
				Texture* pTexture;
			};

			Gpu& m_Gpu;
			BlendState m_BlendState;
			RasterizerState m_RasterizerState;
			SamplerState m_Sampler;
			InputLayout m_InputLayout;
			Shader m_Shader;

			ConstantBuffer<CB_CamMatPos> m_CameraConstantBuffer;
			Array<DrawEntry> m_Entries;
		};
	}
}
