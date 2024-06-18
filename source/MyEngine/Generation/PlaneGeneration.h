#pragma once
#include "DataStructures/List.h"
#include "Geometry/Shapes/Line.h"
#include "Geometry/Shapes/Rect3.h"
#include "Geometry/Shapes/Rects.h"
#include "Rendering/Structs/VertexTypes.h"

namespace MyEngine
{
	namespace Rendering
	{
		class Mesh;
	}
	namespace Generation
	{
		class PlaneGeneration
		{
		public:
			static Array<Rendering::V_Pos2Uv> CreateVertices(const Float2& leftBottom, const Float2& size, const RectFloat& uvs);

			static Array<Rendering::V_PosUv> TowardsZMin(const Float3& leftBottom, const Float2& size);
			static Rendering::Mesh* MeshTowardsZMin(const Float3& leftBottom, const Float2& size);

			static void TowardXMin(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardXMax(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardYMin(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardYMax(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardZMin(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardZMax(
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);

			static void TowardXMin(
				const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardXMax(
				const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardYMin(
				const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardYMax(
				const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardZMin(
				const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void TowardZMax(
				const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);

			static void Create(
				const Float3& right, const Float3& up,
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void Create(
				const Float3& right, const Float3& up, const Float3& normal,
				const Float3& leftBot, const Float2& size, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void Create(
				const Float3& leftBot, const Float3& leftTop,
				const Float3& rightTop, const Float3& rightBot,
				const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);
			static void Create(
				const Rect3Float& rect, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);

			static void CreateBorder(
				const Rect3Float& outer, const Rect3Float& inner, const Float3& color,
				List<Rendering::V_PosColNorm>& vertices, List<int>& indices);

			//---| TEMPLATED |---
			template<typename T>
			static void TowardXMin_PosNor(
				const Float3& leftBot, const Float2& size,
				List<T>& vertices, List<int>& indices
			);
			template<typename T>
			static void TowardXMax_PosNor(
				const Float3& leftBot, const Float2& size,
				List<T>& vertices, List<int>& indices
			);
			template<typename T>
			static void TowardYMin_PosNor(
				const Float3& leftBot, const Float2& size,
				List<T>& vertices, List<int>& indices
			);
			template<typename T>
			static void TowardYMax_PosNor(
				const Float3& leftBot, const Float2& size,
				List<T>& vertices, List<int>& indices
			);
			template<typename T>
			static void TowardZMin_PosNor(
				const Float3& leftBot, const Float2& size,
				List<T>& vertices, List<int>& indices
			);
			template<typename T>
			static void TowardZMax_PosNor(
				const Float3& leftBot, const Float2& size,
				List<T>& vertices, List<int>& indices
			);
			template<typename T>
			static void Create_PosNor(
				const Line& bot, const Line& top,
				List<T>& vertices, List<int>& indices
			);

		private:
			template<typename Vertex, unsigned Axis, int Normal>
			static void TowardAxis_PosNor(
				const Float3& leftBot, const Float2& size,
				List<Vertex>& vertices, List<int>& indices
			);
		};

		template <typename T>
		void PlaneGeneration::TowardXMin_PosNor(const Float3& leftBot, const Float2& size, List<T>& vertices,
			List<int>& indices)
		{
			TowardAxis_PosNor<T, 0, -1>(leftBot, size, vertices, indices);
		}

		template <typename T>
		void PlaneGeneration::TowardXMax_PosNor(const Float3& leftBot, const Float2& size, List<T>& vertices,
			List<int>& indices)
		{
			TowardAxis_PosNor<T, 0, 1>(leftBot, size, vertices, indices);
		}

		template <typename T>
		void PlaneGeneration::TowardYMin_PosNor(const Float3& leftBot, const Float2& size, List<T>& vertices,
			List<int>& indices)
		{
			TowardAxis_PosNor<T, 1, -1>(leftBot, size, vertices, indices);
		}

		template <typename T>
		void PlaneGeneration::TowardYMax_PosNor(const Float3& leftBot, const Float2& size,
			List<T>& vertices, List<int>& indices)
		{
			TowardAxis_PosNor<T, 1, 1>(leftBot, size, vertices, indices);
		}

		template<typename T>
		void PlaneGeneration::TowardZMin_PosNor(const Float3& leftBot, const Float2& size,
			List<T>& vertices, List<int>& indices)
		{
			TowardAxis_PosNor<T, 2, -1 >(leftBot, size, vertices, indices);
		}

		template <typename T>
		void PlaneGeneration::TowardZMax_PosNor(const Float3& leftBot, const Float2& size, List<T>& vertices,
			List<int>& indices)
		{
			TowardAxis_PosNor<T, 2, 1>(leftBot, size, vertices, indices);
		}

		template <typename Vertex>
		void PlaneGeneration::Create_PosNor(const Line& bot, const Line& top, List<Vertex>& vertices, List<int>& indices)
		{
			indices.EnsureIncrease(6);
			const unsigned first{ vertices.GetSize() };
			indices.Add(first, first + 1, first + 2);
			indices.Add(first, first + 2, first + 3);
			vertices.EnsureIncrease(4);

			const Float3 normal{ ((top.a - bot.a).Cross(bot.b - bot.a).Normalized()) };

			//left-bot
			Vertex vertex{};
			vertex.Normal = normal;
			vertex.Position = bot.a;
			vertices.Add(vertex);
			//left-top
			vertex.Position = top.a;
			vertices.Add(vertex);
			//right-top
			vertex.Position = top.b;
			vertices.Add(vertex);
			//right-bot
			vertex.Position = bot.b;
			vertices.Add(vertex);
		}

		template <typename Vertex, unsigned Axis, int Normal>
		void PlaneGeneration::TowardAxis_PosNor(const Float3& leftBot, const Float2& size, List<Vertex>& vertices,
			List<int>& indices)
		{
			indices.EnsureIncrease(6);
			const unsigned first{ vertices.GetSize() };
			indices.Add(first, first + 1, first + 2);
			indices.Add(first, first + 2, first + 3);
			vertices.EnsureIncrease(4);

			const unsigned upAxis{ (Axis + 1) % 3 };
			const unsigned rightAxis{ (Axis + 2) % 3 };
			//left-bot
			Vertex vertex{};
			vertex.Position = leftBot;
			vertex.Normal = Float3::FromComponent(Axis, Normal, 0);
			vertices.Add(vertex);
			//left-top
			vertex.Position[upAxis] += size.y;
			vertices.Add(vertex);
			//right-top
			vertex.Position[rightAxis] += size.x * Normal;
			vertices.Add(vertex);
			//right-bot
			vertex.Position[upAxis] = leftBot[upAxis];
			vertices.Add(vertex);
		}
	}
}
