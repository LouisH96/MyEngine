#pragma once
#include "DataStructures/List.h"
#include "Rendering/DrawData/IndexedVertexArray.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/Shader.h"

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
			explicit DebugSpheres();
			~DebugSpheres() = default;

			void Render();

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
			Rendering::IndexedVertexArray m_Vertices;
		};
	}
}
