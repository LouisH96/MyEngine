#pragma once
#include "IDrawBatch.h"
#include <d3d11.h>

#include "Rendering/Dx/DxHelper.h"

namespace MyEngine
{
	namespace Rendering
	{
		//VertexBuffer & IndexBuffer & InstanceBuffer
		class IndexedDrawBatch final : IDrawBatch
		{
		public:
			//---| Constructor/Destructor |---
			IndexedDrawBatch() = default;
			template<typename Vertex, typename Instance>
			IndexedDrawBatch(
				Gpu& gpu,
				const Array<Vertex>& vertices,
				const Array<Instance>& instances,
				const Array<int>& indices,
				bool verticesImmutable = true,
				bool instancesImmutable = true,
				bool indicesImmutable = true);
			~IndexedDrawBatch() override;

			//---| Copy/Move |---
			IndexedDrawBatch(const IndexedDrawBatch& other) = delete;
			IndexedDrawBatch& operator=(const IndexedDrawBatch& other) = delete;
			IndexedDrawBatch(IndexedDrawBatch&& other) noexcept;
			IndexedDrawBatch& operator=(IndexedDrawBatch&& other) noexcept;

			//---| Functions |---
			void Draw(const Gpu& gpu) override;

			void SetInstancesDrawCount(int count) { m_DrawCountInstances = count; }

			template<typename T>
			void UpdateInstancesData(const Gpu& gpu, T* pData, int count);

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
		IndexedDrawBatch::IndexedDrawBatch(
			Gpu& gpu,
			const Array<Vertex>& vertices,
			const Array<Instance>& instances,
			const Array<int>& indices,
			bool verticesImmutable,
			bool instancesImmutable,
			bool indicesImmutable)
			: m_Strides{ sizeof(Vertex), sizeof(Instance), sizeof(int) }
			, m_Offsets{ 0,0,0 }
			, m_Sizes{ vertices.GetSizeU(), instances.GetSizeU(), indices.GetSizeU() }
			, m_DrawCountInstances{ instances.GetSizeU() }
			, m_DrawCountIndices{ indices.GetSizeU() }
		{
			Dx::DxHelper::CreateVertexBuffer(gpu, m_pBuffers[IDX_VERTICES], vertices, verticesImmutable);
			Dx::DxHelper::CreateInstanceBuffer(gpu, m_pBuffers[IDX_INSTANCES], instances, instancesImmutable);
			Dx::DxHelper::CreateIndexBuffer(gpu, m_pBuffers[IDX_INDICES], indices, indicesImmutable);
		}

		template <typename T>
		void IndexedDrawBatch::UpdateInstancesData(const Gpu& gpu, T* pData, int count)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			gpu.GetContext().Map(m_pBuffers[IDX_INSTANCES], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, pData, count * sizeof(T));
			gpu.GetContext().Unmap(m_pBuffers[IDX_INSTANCES], 0);
		}
	}
}
