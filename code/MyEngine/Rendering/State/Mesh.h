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
			static Mesh* Create(const Gpu& gpu, const Ds::Array<Vertex>& vertices, const Ds::Array<int>& indices, Topology topology = Topology::TriangleList, bool immutable = true);
			template<typename Vertex>
			static Mesh* Create(const Gpu& gpu, const Ds::Array<Vertex>& vertices, Topology topology = Topology::TriangleList, bool immutable = true);
			template<typename Vertex>
			static Mesh* Create(const Ds::Array<Vertex>& vertices, const Ds::Array<int>& indices, Topology topology = Topology::TriangleList, bool immutable = true);
			template<typename Vertex>
			static Mesh* Create(const Ds::Array<Vertex>& vertices, Topology topology = Topology::TriangleList, bool immutable = true);
			~Mesh();

			template<typename T>
			void Update(const Gpu& gpu, const Array<T>& data);

			void Activate(const Gpu& gpu) const;
			void Draw(const Gpu& gpu) const;

		private:
			typedef void (Mesh::* FpActivate)(const Gpu&) const;
			typedef void (Mesh::* FpDraw)(const Gpu&) const;
			ID3D11Buffer* m_pVertexBuffer{};
			ID3D11Buffer* m_pIndexBuffer{};
			unsigned int m_VertexCount{};
			unsigned int m_VertexStride{};
			unsigned int m_VertexOffset{};
			unsigned int m_IndexCount{};
			D3D11_PRIMITIVE_TOPOLOGY m_Topology{};
			FpActivate m_fpActivate{};
			FpDraw m_fpDraw{};

			Mesh(const Gpu& gpu, unsigned vertexStride, const void* pVertices, int nrVertices, const int* pIndices, int nrIndices, Topology topology = Topology::TriangleList, bool immutable = false);
			Mesh(const Gpu& gpu, unsigned vertexStride, const void* pVertices, int nrVertices, Topology topology = Topology::TriangleList, bool immutable = false);

			void InitVertexBuffer(const Gpu& gpu, const void* pInitVertices, bool immutable);

			void ActivateIndexed(const Gpu& gpu) const;
			void ActivateUnindexed(const Gpu& gpu) const;
			void DrawIndexed(const Gpu& gpu) const;
			void DrawUnindexed(const Gpu& gpu) const;

			static D3D11_PRIMITIVE_TOPOLOGY ToDxTopology(Topology topology);
		};

		template <typename Vertex>
		Mesh* Mesh::Create(const Gpu& gpu, const Ds::Array<Vertex>& vertices, const Ds::Array<int>& indices,
			Topology topology, bool immutable)
		{
			return new Mesh(gpu, sizeof(Vertex), vertices.GetData(), vertices.GetSize(), indices.GetData(), indices.GetSize(), topology, immutable);
		}

		template <typename Vertex>
		Mesh* Mesh::Create(const Gpu& gpu, const Ds::Array<Vertex>& vertices, Topology topology, bool immutable)
		{
			return new Mesh(gpu, sizeof(Vertex), vertices.GetData(), vertices.GetSize(), topology, immutable);
		}

		template <typename Vertex>
		Mesh* Mesh::Create(const Ds::Array<Vertex>& vertices, const Ds::Array<int>& indices, Topology topology,
			bool immutable)
		{
			return Create(*Globals::pGpu, vertices, indices, topology, immutable);
		}

		template <typename Vertex>
		Mesh* Mesh::Create(const Ds::Array<Vertex>& vertices, Topology topology, bool immutable)
		{
			return Create(*Globals::pGpu, vertices, topology, immutable);
		}

		template <typename T>
		void Mesh::Update(const Gpu& gpu, const Array<T>& data)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			gpu.GetContext().Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, data.GetData(), sizeof(T)*data.GetSize() );
			gpu.GetContext().Unmap(m_pVertexBuffer, 0);
		}
	}
}
