#pragma once
#include "Rendering/State/BlendState.h"
#include "Rendering/State/ConstantBuffer.h"
#include "Rendering/State/IndexedDrawBatch.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/RasterizerState.h"
#include "Rendering/State/Shader.h"
#include "Rendering/Structs/ConstantBufferTypes.h"

namespace MyEngine
{
	namespace Rendering
	{
		class Mesh;
	}
}

namespace MyEngine
{
	namespace Debug
	{
		class DebugSphereInstancingRenderer
		{
		public:
			explicit DebugSphereInstancingRenderer(Rendering::Gpu& gpu);
			~DebugSphereInstancingRenderer() = default;

			void Render(const Rendering::Gpu& gpu, const Float3& cameraPosition, const Float4X4& viewProjection);

		private:
			Rendering::BlendState m_BlendState;
			Rendering::RasterizerState m_RasterizerState;
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;
			Rendering::ConstantBuffer<Rendering::CB_CamMatPos> m_ConstantBuffer;
			static const Rendering::InputLayout::Element ELEMENTS[];

			Rendering::IndexedDrawBatch m_DrawBatch;
		};
	}
}
