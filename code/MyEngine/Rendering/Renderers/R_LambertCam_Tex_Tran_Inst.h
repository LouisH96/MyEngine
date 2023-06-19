#pragma once
#include "Rendering/DrawData/InstanceList.h"
#include "Rendering/State/BlendState.h"
#include "Rendering/State/ConstantBuffer.h"
#include "Rendering/State/DepthStencilState.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/RasterizerState.h"
#include "Rendering/State/SamplerState.h"
#include "Rendering/State/Shader.h"
#include "Rendering/State/Texture.h"
#include "Rendering/Structs/ConstantBufferTypes.h"
#include "Rendering/Structs/Instances.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace Rendering
	{
		class R_LambertCam_Tex_Tran_Inst
		{
		public:
			using Vertex = V_PosNorUv;
			using Instance = I_ModelMatrices;

			R_LambertCam_Tex_Tran_Inst();
			~R_LambertCam_Tex_Tran_Inst() = default;

			void Render();

			int CreateModel(Texture&& texture, const Vertex* pVertices, unsigned nrVertices);
			void AddInstance(int modelIdx, const Transform& instanceTransform);

		private:
			BlendState m_BlendState;
			RasterizerState m_RasterizerState;
			SamplerState m_Sampler;
			DepthStencilState m_DepthStencilState;
			ConstantBuffer<CB_CamPos> m_CameraBuffer;

			InputLayout m_InputLayout;
			Shader m_Shader;

			List<Texture> m_Textures;
			List<InstanceList<Vertex, Instance>> m_InstanceLists;
			List<List<const Transform*>> m_Transforms;
		};
	}
}
