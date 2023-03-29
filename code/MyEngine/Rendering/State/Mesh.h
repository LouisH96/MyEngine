#pragma once
#include "../Gpu.h"
#include "Shader.h"
#include "../Dx/DxHelper.h"
#include "DataStructures/Array.h"

namespace MyEngine
{
	namespace Rendering
	{
		//---| Nested |---
		enum class Topology
		{
			TriangleList, LineStrip, Unknown
		};

		//---| Class |---
		class Mesh
		{
		public:
			Mesh(const Mesh& other) = delete;
			Mesh(Mesh&& other) noexcept = delete;
			Mesh& operator=(const Mesh& other) = delete;
			Mesh& operator=(Mesh&& other) noexcept = delete;

			template<typename Vertex>
			static Mesh* Create(const Gpu& gpu, const Ds::Array<Vertex>& vertices, const Ds::Array<int>& indices, Topology topology = Topology::TriangleList);
			template<typename Vertex>
			static Mesh* Create(const Gpu& gpu, const Ds::Array<Vertex>& vertices, Topology topology = Topology::TriangleList);
			~Mesh();

			void Activate(const Gpu& gpu) const;
			void ActivateUnindexed(const Gpu& gpu) const;
			void Draw(const Gpu& gpu) const;
			void DrawNotIndexed(const Gpu& gpu) const;

		private:
			ID3D11Buffer* m_pVertexBuffer{};
			ID3D11Buffer* m_pIndexBuffer{};
			unsigned int m_VertexCount{};
			unsigned int m_VertexStride{};
			unsigned int m_VertexOffset{};
			unsigned int m_IndexCount{};
			D3D11_PRIMITIVE_TOPOLOGY m_Topology{};

			Mesh(const Gpu& gpu, unsigned vertexStride, const void* pVertices, int nrVertices, const int* pIndices, int nrIndices, Topology topology = Topology::TriangleList);
			static D3D11_PRIMITIVE_TOPOLOGY ToDxTopology(Topology topology);
		};

		template <typename Vertex>
		Mesh* Mesh::Create(const Gpu& gpu, const Ds::Array<Vertex>& vertices, const Ds::Array<int>& indices,
			Topology topology)
		{
			return new Mesh(gpu, sizeof(Vertex), vertices.GetData(), vertices.GetSize(), indices.GetData(), indices.GetSize(), topology);
		}

		template <typename Vertex>
		Mesh* Mesh::Create(const Gpu& gpu, const Ds::Array<Vertex>& vertices, Topology topology)
		{
			return new Mesh(gpu, sizeof(Vertex), vertices.GetData(), vertices.GetSize(), nullptr, 0, topology);
		}
	}
}
