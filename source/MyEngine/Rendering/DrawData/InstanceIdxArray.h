#pragma once
#include <Rendering/Dx/DxHelper.h>

struct ID3D11Buffer;

namespace MyEngine
{
	namespace Rendering
	{
		class InstanceIdxArray
		{
		public:
			//---| Constructor/Destructor |---
			InstanceIdxArray();
			~InstanceIdxArray();
			template<typename Vertex>
			InstanceIdxArray(
				const Vertex* pVertices, unsigned nrVertices,
				const int* pIndices, unsigned nrIndices,
				unsigned instanceStride, unsigned instanceCapacity = 5, bool dynamicInstances = false);
			template<typename Vertex, typename Instance>
			InstanceIdxArray(
				const Vertex* pVertices, unsigned nrVertices,
				const int* pIndices, unsigned nrIndices,
				const Instance* pInstances, unsigned nrInstances, bool dynamicInstances = false);

			//---| Copy/Move |---
			InstanceIdxArray(const InstanceIdxArray& other) = delete;
			InstanceIdxArray(InstanceIdxArray&& other) noexcept;
			InstanceIdxArray& operator=(const InstanceIdxArray& other) = delete;
			InstanceIdxArray& operator=(InstanceIdxArray&& other) noexcept;

			//---| Functions |---
			void Draw() const;
			void Draw(unsigned count) const;
			void SetInstancesCount(unsigned count);
			void SetInstanceCapacity(unsigned capacity, bool dynamic);
			void EnsureCapacity(unsigned minCapacity, bool dynamic);

			template<typename Instance> void UpdateInstances(const Instance* pInstances, unsigned nrInstances);

		private:
			static constexpr int NR_BUFFERS = 3;
			static constexpr int IDX_VERTICES = 0;
			static constexpr int IDX_INSTANCES = 1;
			static constexpr int IDX_INDICES = 2;

			ID3D11Buffer* m_pBuffers[NR_BUFFERS];
			unsigned m_Strides[NR_BUFFERS]{};
			unsigned m_Offsets[NR_BUFFERS]{};
			unsigned m_Counts[NR_BUFFERS]{};
			unsigned m_Capacities[NR_BUFFERS]{};
		};

		template <typename Vertex>
		InstanceIdxArray::InstanceIdxArray(const Vertex* pVertices, unsigned nrVertices, const int* pIndices,
			unsigned nrIndices, unsigned instanceStride, unsigned instanceCapacity, bool dynamicInstances)
			: m_Strides{ sizeof(Vertex), instanceStride, sizeof(int) }
			, m_Offsets{ 0,0,0 }
			, m_Counts{ nrVertices, 0, nrIndices }
			, m_Capacities{ nrVertices, instanceCapacity, nrIndices }
		{
			Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_VERTICES], pVertices, nrVertices, true);
			Dx::DxHelper::CreateIndexBuffer(m_pBuffers[IDX_INDICES], pIndices, nrIndices, true);
			Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_INSTANCES], instanceStride, instanceCapacity, !dynamicInstances);
		}

		template <typename Vertex, typename Instance>
		InstanceIdxArray::InstanceIdxArray(const Vertex* pVertices, unsigned nrVertices, const int* pIndices,
			unsigned nrIndices, const Instance* pInstances, unsigned nrInstances, bool dynamicInstances)
			: m_Strides{ sizeof(Vertex), sizeof(Instance), sizeof(int) }
			, m_Offsets{ 0,0,0 }
			, m_Counts{ nrVertices, nrInstances, nrIndices }
			, m_Capacities{ nrVertices, nrInstances, nrIndices }
		{
			Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_VERTICES], pVertices, nrVertices, true);
			Dx::DxHelper::CreateIndexBuffer(m_pBuffers[IDX_INDICES], pIndices, nrIndices, true);
			Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_INSTANCES], pInstances, nrInstances, !dynamicInstances);
		}

		template <typename Instance>
		void InstanceIdxArray::UpdateInstances(const Instance* pInstances, unsigned nrInstances)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			const HRESULT result{ Globals::pGpu->GetContext().Map(m_pBuffers[IDX_INSTANCES], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) };
			if (FAILED(result)) Logger::PrintError("Failed updating instances in InstanceIdxArray");
			memcpy(mappedResource.pData, pInstances, nrInstances * sizeof(Instance));
			Globals::pGpu->GetContext().Unmap(m_pBuffers[IDX_INSTANCES], 0);
		}
	}
}
