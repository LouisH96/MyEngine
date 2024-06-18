#pragma once

#include "Rendering/Dx/DxHelper.h"

struct ID3D11Buffer;

namespace MyEngine
{
	namespace Rendering
	{
		class VertexIdxArray
		{
		public:
			//---| Constructor/Destructor |---
			VertexIdxArray();
			~VertexIdxArray();

			VertexIdxArray(unsigned vertexStride, unsigned vertexCapacity, unsigned indexCapacity);
			template<typename Vertex>
			VertexIdxArray(
				const Vertex* pVertices, unsigned nrVertices, bool dynamicVertices,
				const int* pIndices, unsigned nrIndices, bool dynamicIndices);

			//---| Copy/Move |---
			VertexIdxArray(const VertexIdxArray& other) = delete;
			VertexIdxArray& operator=(const VertexIdxArray& other) = delete;
			VertexIdxArray(VertexIdxArray&& other) noexcept;
			VertexIdxArray& operator=(VertexIdxArray&& other) noexcept;

			//---| Functions |---
			void Draw(unsigned idx = 0) const;

			void SetVertexCount(unsigned count);
			void SetIndexCount(unsigned count);

			void EnsureVertexCapacity(unsigned minCapacity, bool dynamic);
			void EnsureIndexCapacity(unsigned minCapacity, bool dynamic);

			template<typename Vertex>
			void UpdateVertices(const Vertex* pVertices, unsigned nrVertices);
			void UpdateIndices(const int* pIndices, unsigned nrIndices);

		private:
			static constexpr int NR_BUFFERS = 2;
			static constexpr int IDX_VERTICES = 0;
			static constexpr int IDX_INDICES = 1;

			ID3D11Buffer* m_pBuffers[NR_BUFFERS];
			unsigned m_Strides[NR_BUFFERS];
			unsigned m_Offsets[NR_BUFFERS];
			unsigned m_Counts[NR_BUFFERS];
			unsigned m_Capacities[NR_BUFFERS];

			void SetVertexCapacity(unsigned capacity, bool dynamic);
			void SetIndexCapacity(unsigned capacity, bool dynamic);
		};

		template <typename Vertex>
		VertexIdxArray::VertexIdxArray(const Vertex* pVertices, unsigned nrVertices, bool dynamicVertices,
			const int* pIndices, unsigned nrIndices, bool dynamicIndices)
			: m_pBuffers{ nullptr, nullptr }
			, m_Strides{ sizeof(Vertex), sizeof(int) }
			, m_Offsets{ 0,0 }
			, m_Counts{ nrVertices, nrIndices }
			, m_Capacities{ nrVertices, nrIndices }
		{
			Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_VERTICES], pVertices, static_cast<int>(nrVertices), !dynamicVertices);
			Dx::DxHelper::CreateIndexBuffer(m_pBuffers[IDX_INDICES], pIndices, static_cast<int>(nrIndices), !dynamicIndices);
		}

		template <typename Vertex>
		void VertexIdxArray::UpdateVertices(const Vertex* pVertices, unsigned nrVertices)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			const HRESULT result{ Globals::pGpu->GetContext().Map(m_pBuffers[IDX_VERTICES], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) };
			if (FAILED(result)) Logger::PrintError("[VertexIdxArray] Failed updating vertices");
			memcpy(mappedResource.pData, pVertices, nrVertices * sizeof(Vertex));
			Globals::pGpu->GetContext().Unmap(m_pBuffers[IDX_VERTICES], 0);
		}
	}
}
