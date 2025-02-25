#pragma once
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
			static Mesh* Create(const Array<Vertex>& vertices, const Array<int>& indices, Topology topology = Topology::TriangleList, bool immutable = true);
			template<typename Vertex>
			static Mesh* Create(const Array<Vertex>& vertices, Topology topology = Topology::TriangleList, bool immutable = true);
			~Mesh();

			template<typename T>
			void Update(const Array<T>& data);

			void Activate() const;
			void Draw() const;

		private:
			typedef void (Mesh::* FpActivate)() const;
			typedef void (Mesh::* FpDraw)() const;
			ID3D11Buffer* m_pVertexBuffer{};
			ID3D11Buffer* m_pIndexBuffer{};
			unsigned int m_VertexCount{};
			unsigned int m_VertexStride{};
			unsigned int m_VertexOffset{};
			unsigned int m_IndexCount{};
			D3D11_PRIMITIVE_TOPOLOGY m_Topology{};
			FpActivate m_fpActivate{};
			FpDraw m_fpDraw{};

			Mesh(unsigned vertexStride, const void* pVertices, int nrVertices, const int* pIndices, int nrIndices, Topology topology = Topology::TriangleList, bool immutable = false);
			Mesh(unsigned vertexStride, const void* pVertices, int nrVertices, Topology topology = Topology::TriangleList, bool immutable = false);

			void InitVertexBuffer(const void* pInitVertices, bool immutable);

			void ActivateIndexed() const;
			void ActivateUnindexed() const;
			void DrawIndexed() const;
			void DrawUnindexed() const;

			static D3D11_PRIMITIVE_TOPOLOGY ToDxTopology(Topology topology);
		};

		template <typename Vertex>
		Mesh* Mesh::Create(const Array<Vertex>& vertices, const Array<int>& indices, Topology topology,
			bool immutable)
		{
			return new Mesh(sizeof(Vertex), vertices.GetData(), vertices.GetSize(), indices.GetData(), indices.GetSize(), topology, immutable);
		}

		template <typename Vertex>
		Mesh* Mesh::Create(const Array<Vertex>& vertices, Topology topology, bool immutable)
		{
			return new Mesh(sizeof(Vertex), vertices.GetData(), vertices.GetSize(), topology, immutable);
		}

		template <typename T>
		void Mesh::Update(const Array<T>& data)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			const HRESULT result{ Globals::pGpu->GetContext().Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) };
			if (FAILED(result)) Logger::Error("[Mesh] Failed mapping vertex-buffer");
			memcpy(mappedResource.pData, data.GetData(), sizeof(T)*data.GetSize() );
			Globals::pGpu->GetContext().Unmap(m_pVertexBuffer, 0);
		}
	}
}
