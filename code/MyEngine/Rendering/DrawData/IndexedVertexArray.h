#pragma once
#include "IDrawData.h"
#include <d3d11.h>

#include "DataStructures/List.h"
#include "Rendering/Dx/DxHelper.h"

namespace MyEngine
{
	namespace Rendering
	{
		//VertexBuffer & IndexBuffer & InstanceBuffer
		class IndexedVertexArray final : IDrawData
		{
		public:
			//---| Constructor/Destructor |---
			IndexedVertexArray() = default;
			template<typename Vertex, typename Instance>
			IndexedVertexArray(
				Gpu& gpu,
				const Vertex* pVertices, int nrVertices,
				const Instance* pInstances, int nrInstances,
				const int* pIndices, int nrIndices,
				bool verticesImmutable = true,
				bool instancesImmutable = true,
				bool indicesImmutable = true);
			template<typename Vertex, typename Instance>
			IndexedVertexArray(
				Gpu& gpu,
				const Array<Vertex>& vertices,
				const Array<Instance>& instances,
				const Array<int>& indices,
				bool verticesImmutable = true,
				bool instancesImmutable = true,
				bool indicesImmutable = true);
			~IndexedVertexArray() override;

			//---| Copy/Move |---
			IndexedVertexArray(const IndexedVertexArray& other) = delete;
			IndexedVertexArray& operator=(const IndexedVertexArray& other) = delete;
			IndexedVertexArray(IndexedVertexArray&& other) noexcept;
			IndexedVertexArray& operator=(IndexedVertexArray&& other) noexcept;

			//---| Functions |---
			void Draw(const Gpu& gpu) override;

			unsigned int GetInstancesSize() const;
			void SetInstancesDrawCount(int count) { m_DrawCountInstances = count; }

			template<typename T> void UpdateInstancesData(const Gpu& gpu, T* pData, int count);
			template<typename T> void UpdateInstancesData(const Gpu& gpu, const List<T>& list);
			template<typename T> void RecreateInstancesWithCapacity(const Gpu& gpu, const List<T>& list, bool immutable);

		private:
			static constexpr int NR_BUFFERS = 3;
			static constexpr int IDX_VERTICES = 0;
			static constexpr int IDX_INSTANCES = 1;
			static constexpr int IDX_INDICES = 2;
			ID3D11Buffer* m_pBuffers[NR_BUFFERS];
			unsigned int m_Strides[NR_BUFFERS];
			unsigned int m_Offsets[NR_BUFFERS];
			unsigned int m_Sizes[NR_BUFFERS];
			unsigned int m_DrawCountInstances;
			unsigned int m_DrawCountIndices;
		};

		template <typename Vertex, typename Instance>
		IndexedVertexArray::IndexedVertexArray(
			Gpu& gpu,
			const Vertex* pVertices, int nrVertices,
			const Instance* pInstances, int nrInstances,
			const int* pIndices, int nrIndices,
			bool verticesImmutable, bool instancesImmutable, bool indicesImmutable)
			: m_Strides{ sizeof(Vertex), sizeof(Instance), sizeof(int) }
			, m_Offsets{ 0,0,0 }
			, m_Sizes{ static_cast<unsigned>(nrVertices), static_cast<unsigned>(nrInstances), static_cast<unsigned>(nrIndices) }
			, m_DrawCountInstances{ static_cast<unsigned>(nrInstances) }
			, m_DrawCountIndices{ static_cast<unsigned>(nrIndices) }
		{
			Dx::DxHelper::CreateVertexBuffer(gpu, m_pBuffers[IDX_VERTICES], pVertices, nrVertices, verticesImmutable);
			Dx::DxHelper::CreateInstanceBuffer(gpu, m_pBuffers[IDX_INSTANCES], pInstances, nrInstances, instancesImmutable);
			Dx::DxHelper::CreateIndexBuffer(gpu, m_pBuffers[IDX_INDICES], pIndices, nrIndices, indicesImmutable);
		}

		template <typename Vertex, typename Instance>
		IndexedVertexArray::IndexedVertexArray(
			Gpu& gpu,
			const Array<Vertex>& vertices,
			const Array<Instance>& instances,
			const Array<int>& indices,
			bool verticesImmutable,
			bool instancesImmutable,
			bool indicesImmutable)
			: IndexedVertexArray{ gpu,
				vertices.GetData(), vertices.GetSize(),
				instances.GetData(), instances.GetSize(),
				indices.GetData(), indices.GetSize(),
				verticesImmutable, instancesImmutable, indicesImmutable }
		{
		}

		template <typename T>
		void IndexedVertexArray::UpdateInstancesData(const Gpu& gpu, T* pData, int count)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			gpu.GetContext().Map(m_pBuffers[IDX_INSTANCES], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, pData, count * sizeof(T));
			gpu.GetContext().Unmap(m_pBuffers[IDX_INSTANCES], 0);
		}

		template <typename T>
		void IndexedVertexArray::UpdateInstancesData(const Gpu& gpu, const List<T>& list)
		{
			UpdateInstancesData(gpu, list.GetData(), list.GetSize());
		}

		template <typename T>
		void IndexedVertexArray::RecreateInstancesWithCapacity(const Gpu& gpu, const List<T>& list, bool immutable)
		{
			m_pBuffers[IDX_INSTANCES]->Release();
			Dx::DxHelper::CreateInstanceBuffer(gpu, m_pBuffers[IDX_INSTANCES], list.GetData(), list.GetCapacity(), immutable);
			m_Sizes[IDX_INSTANCES] = list.GetCapacity();
		}
	}
}
