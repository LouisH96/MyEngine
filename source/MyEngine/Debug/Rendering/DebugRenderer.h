#pragma once
#include <Math/Vectors.h>
#include <Rendering/Renderers/RendererFactory.h>
#include <Rendering/State/BlendState.h>
#include <Rendering/State/ConstantBuffer.h>
#include <Rendering/State/InputLayout.h>
#include <Rendering/State/RasterizerState.h>
#include <Rendering/State/Shader.h>
#include <Rendering/Structs/ConstantBufferTypes.h>
#include <Rendering/Structs/VertexTypes.h>

#include "DebugCones.h"
#include "DebugCubes.h"
#include "DebugLines.h"
#include "DebugSpheres.h"
#include "DebugTriangles.h"

namespace MyEngine
{
	template<typename> class Array;
	class Cone;
	class Sphere;
	class CubeAA;
	struct Line;

	//---| Forward Includes |---
	namespace Rendering
	{
		class Mesh;
		class Gpu;
	}

	//---| Class |---
	namespace Debug
	{

		class DebugRenderer
		{
		public:
			//---| Rule of Five |---
			DebugRenderer(const DebugRenderer& other) = delete;
			DebugRenderer(DebugRenderer&& other) noexcept = delete;
			DebugRenderer& operator=(const DebugRenderer& other) = delete;
			DebugRenderer& operator=(DebugRenderer&& other) noexcept = delete;

			static void Init();
			static void Release();
			static void Render();

			static void AddSphere(const Float3& position, const Float3& color, float radius);
			static void AddSpheres(const Array<Float3>& positions, const Float3& color, float radius);

			static void AddLine(const Float3& begin, const Float3& end, const Float3& color);
			static void AddLine(const Array<Float3>& points, const Float3& color);

			static void DrawLine(const Float3& begin, const Float3& end, const Float3& color);
			static void DrawLine(const Line& line, const Float3& color);
			static void DrawLine(PtrRangeConst<Float3> points, const Float3& color);
			static void DrawRay(const Float3& origin, const Float3& displacement, const Float3& color);
			static void DrawRay(const Ray& ray, const Float3& color);
			static void DrawTriangle(const Triangle& triangle, const Float3& color);
			static void DrawTriangle(const Float3* pTriangle, const Float3& color);
			static void DrawTriangle(const Float3* pTriangle, const Float3& normal, const Float3& color);

			static void AddRay(const Float3& origin, const Float3& displacement, const Float3& color);

			static void AddRect(const Float3& leftTop, const Float3& rightTop, const Float3& rightBot, const Float3& leftBot, const
				Float3& color);
			template<typename T> static void AddRect(const Vector2<T>& min, const Vector2<T>& max, const Float3& offset, const
				Float3& color);

			static void AddGridXy(const Float3& offset, const Float2& bounds, int nrCols, int nrRows, const Float3& color);

			static void DrawSphere(const Float3& position, const Float3& color, float radius);
			static void DrawSphere(const Sphere& sphere, const Float3& color);

			static void DrawDiskXz(const Float3& center, float radius, const Float3& color);

			static void DrawCube(const Cube& cube, const Float3& color);
			static void DrawCube(const CubeAA& cube, const Float3& color);

			static void DrawCone(const Cone& cone, const Float3& color);

		private:
			using Vertex = Rendering::V_PosColNorm;
			using LineRenderer = Rendering::RendererFactory::UnlitRenderer;
			using LineVertex = LineRenderer::VertexType;

			//---| General |---
			static DebugRenderer* m_pStatic;

			//---| RenderState |---
			Rendering::DepthStencilState m_DepthStencilState;
			Rendering::BlendState m_BlendState;
			Rendering::RasterizerState m_RasterizerState;
			Rendering::InputLayout m_InputLayout;
			Rendering::ConstantBuffer<Rendering::CB_CamMatPos> m_ConstantBuffer;
			Rendering::Shader m_Shader;
			static const Rendering::InputLayout::Element ELEMENTS[];
			LineRenderer* m_pLineRenderer{};
			DebugSpheres m_SpheresRenderer;
			DebugLines m_LinesRenderer2;
			DebugCubes m_CubeRenderer;
			DebugCones m_ConeRenderer;
			DebugTriangles m_TriangleRenderer;

			//---| Mesh |---
			Array<Rendering::Mesh*> m_Meshes{};

			//---| Init |---
			DebugRenderer();
			~DebugRenderer();

			//---| Loop |---
			void Class_Render();

			//---| Operations |---
			void Class_AddSphere(const Float3& position, const Float3& color, float radius);
			void Class_AddSpheres(const Array<Float3>& positions, const Float3& color, float radius);
			void Class_AddLine(const Float3& begin, const Float3& end, const Float3& color) const;
			void Class_AddLine(const Array<Float3>& points, const Float3& color) const;
			void Class_AddRect(const Float3& leftTop, const Float3& rightTop, const Float3& rightBot, const Float3& leftBot, const Float3& color) const;
			template<typename T> void Class_AddRect(const Vector2<T>& min, const Vector2<T>& max, const Float3& offset, const Float3& color);
		};

		template <typename T>
		void DebugRenderer::AddRect(const Vector2<T>& min, const Vector2<T>& max, const Float3& offset, const Float3& color)
		{
			m_pStatic->Class_AddRect(min, max, offset, color);
		}

		template <typename T>
		void DebugRenderer::Class_AddRect(const Vector2<T>& min, const Vector2<T>& max, const Float3& offset,
			const Float3& color)
		{
			const float left{ static_cast<float>(min.x) + offset.x };
			const float right{ static_cast<float>(max.x) + offset.x };
			const float bot{ static_cast<float>(min.y) + offset.y };
			const float top{ static_cast<float>(max.y) + offset.y };
			Array<LineVertex> vertices{ 5 };
			vertices[0] = { {left, bot, offset.z}, color };
			vertices[1] = { {left, top, offset.z}, color };
			vertices[2] = { {right, top, offset.z}, color };
			vertices[3] = { {right, bot, offset.z}, color };
			vertices[4] = vertices[0];
			m_pLineRenderer->AddMesh(Rendering::Mesh::Create(vertices, Rendering::Topology::LineStrip));
		}
	}
}
