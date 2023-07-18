#pragma once
#include "Rendering/DrawData/InstanceIdxList.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/Shader.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	class Sphere;
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
			void DrawSphere(const Sphere& sphere, const Float3& color);

		private:
			using Vertex = Rendering::V_PosNor;
			struct Instance
			{
				Float3 Position;
				Float3 Color;
				float Size;
			};
			Rendering::InputLayout m_InputLayout;
			Rendering::Shader m_Shader;
			static const Rendering::InputLayout::Element ELEMENTS[];

			Rendering::InstanceIdxList<Vertex, Instance> m_Spheres;
		};
	}
}
