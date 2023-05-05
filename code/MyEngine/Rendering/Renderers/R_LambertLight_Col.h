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

		class R_LambertLight_Col
		{
		public:
			//---| Type Alias |---
			using Vertex = V_PosNorCol;

			//---| Constructor/Destructor |---
			explicit R_LambertLight_Col(Gpu& gpu, bool isWireframe = false);
			~R_LambertLight_Col() = default;

			//---| Rule of five |---
			R_LambertLight_Col(const R_LambertLight_Col& other) = delete;
			R_LambertLight_Col(R_LambertLight_Col&& other) noexcept = delete;
			R_LambertLight_Col& operator=(const R_LambertLight_Col& other) = delete;
			R_LambertLight_Col& operator=(R_LambertLight_Col&& other) noexcept = delete;

			//---| Loop |---
			void Render(const Math::Float3& cameraPosition, const Math::Float4X4& viewProjection);

			//---| Operations |---
			void AddEntry(Mesh& mesh);

		private:
			Gpu& m_Gpu;
			BlendState m_BlendState;
			RasterizerState m_RasterizerState;
			SamplerState m_Sampler;
			InputLayout m_InputLayout;
			Shader m_Shader;

			ConstantBuffer<CB_CamMatPos> m_CameraConstantBuffer;
			Array<Mesh*> m_Entries;
		};
	}
}
