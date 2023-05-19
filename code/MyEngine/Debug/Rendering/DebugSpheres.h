#pragma once
#include "DataStructures/List.h"
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
		class DebugSpheres
		{
		public:
			explicit DebugSpheres(Rendering::Gpu& gpu);
			~DebugSpheres() = default;

			void Render(const Rendering::Gpu& gpu, const Float3& cameraPosition, const Float4X4& viewProjection);

			void DrawSphere(const Float3& position, const Float3& color, float size);

		private:
			struct InstanceData
			{
				Float3 Position;
				Float3 Color;
				float Size;
			};
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;
			static const Rendering::InputLayout::Element ELEMENTS[];

			List<InstanceData> m_Spheres{};
			Rendering::IndexedDrawBatch m_DrawBatch;
		};
	}
}
