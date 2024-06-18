#pragma once

#include "DataStructures/List.h"
#include "Rendering/DrawData/InstanceList.h"
#include "Rendering/State/BlendState.h"
#include "Rendering/State/DepthStencilState.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/RasterizerState.h"
#include "Rendering/State/SamplerState.h"
#include "Rendering/State/Shader.h"
#include "Rendering/State/Texture.h"
#include "Rendering/Structs/InstanceTypes.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace Game
	{
		class Camera;
	}

	namespace Rendering
	{
		class Texture2DRenderer
		{
		public:
			//---| Types |---
			using Instance = I_Pos2UvCol; //pos in ndc-space, for now

			//---| Constructor/Destructor |---
			Texture2DRenderer();
			~Texture2DRenderer() = default;

			//---| Functions |---
			void Render();

			int Add(Texture&& texture);
			void Add(int idx, const Instance& instance);
			int Add(Texture&& texture, const Instance& instance);

			Texture& GetTexture(int idx);

		private:
			using Vertex = V_Pos2Uv;

			BlendState m_BlendState;
			RasterizerState m_RasterizerState;
			SamplerState m_SamplerState;
			DepthStencilState m_DepthStencilState;

			InputLayout m_InputLayout;
			Shader m_Shader;

			List<Texture> m_Textures;
			List<InstanceList<Vertex, Instance>> m_Instances;
		};
	}
}
