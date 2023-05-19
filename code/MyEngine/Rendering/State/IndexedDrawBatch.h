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

		private:
			static constexpr int NR_BUFFERS = 3;
			static constexpr int IDX_VERTICES = 0;
			static constexpr int IDX_INSTANCES = 1;
			static constexpr int IDX_INDICES = 2;
			ID3D11Buffer* m_pBuffers[NR_BUFFERS];
			unsigned int m_Strides[NR_BUFFERS];
			unsigned int m_Offsets[NR_BUFFERS];
			unsigned int m_Counts[NR_BUFFERS];
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
			: m_Strides{ sizeof(Vertex), sizeof(Instance), sizeof(int)}
			, m_Offsets{ 0,0,0 }
			, m_Counts{ vertices.GetSizeU(), instances.GetSizeU(), indices.GetSizeU()}
		{
			Dx::DxHelper::CreateVertexBuffer(gpu, m_pBuffers[IDX_VERTICES], vertices, verticesImmutable);
			Dx::DxHelper::CreateInstanceBuffer(gpu, m_pBuffers[IDX_INSTANCES], instances, instancesImmutable);
			Dx::DxHelper::CreateIndexBuffer(gpu, m_pBuffers[IDX_INDICES], indices, indicesImmutable);
		}
	}
}
