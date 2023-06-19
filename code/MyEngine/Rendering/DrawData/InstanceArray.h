#pragma once
#include "IDrawData.h"
#include <Rendering/Dx/DxHelper.h>

struct ID3D11Buffer;

namespace MyEngine
{
	namespace Rendering
	{
		//VertexBuffer & InstanceBuffer (No IndexBuffer)
		class InstanceArray final : public IDrawData
		{
		public:
			//---| Constructor/Destructor |---
			InstanceArray();
			template<typename Vertex>
			InstanceArray(const Vertex* pVertices, unsigned nrVertices, unsigned instanceStride, unsigned instancesCapacity = 5, bool verticesImmutable = true, bool instancesImmutable = true);
			template<typename Vertex, typename Instance>
			InstanceArray(const Vertex* pVertices, unsigned nrVertices, const Instance* pInstances, unsigned nrInstances, bool verticesImmutable = true, bool instancesImmutable = true);
			~InstanceArray() override;

			//---| Copy/Move |---
			InstanceArray(const InstanceArray& other) = delete;
			InstanceArray& operator=(const InstanceArray& other) = delete;
			InstanceArray(InstanceArray&& other) noexcept;
			InstanceArray& operator=(InstanceArray&& other) noexcept;

			//---| Functions |---
			void Draw() override;
			void Draw(unsigned instanceCount) const;

			unsigned GetVertexCount() const { return m_Counts[IDX_VERTICES]; }
			unsigned GetInstanceCount() const { return m_Counts[IDX_INSTANCES]; }

			void SetInstanceCount(unsigned count) { m_Counts[IDX_INSTANCES] = count; }
			void SetInstanceCapacity(unsigned capacity, bool immutable);

			void EnsureInstanceCapacity(unsigned minCapacity, bool immutable);
			template<typename Instance> void UpdateInstances(const Instance* pInstances, unsigned nrInstances);

			template<typename Vertex> void SetVertices(const Vertex* pVertices, unsigned nrVertices, bool immutable = true);
			template<typename Instance> void SetInstances(const Instance* pInstances, unsigned nrInstances, bool immutable = true);

		protected:
			static constexpr int NR_BUFFERS = 2;
			static constexpr int IDX_VERTICES = 0;

			static constexpr int IDX_INSTANCES = 1;
			ID3D11Buffer* m_pBuffers[NR_BUFFERS];
			unsigned m_Strides[NR_BUFFERS];
			unsigned m_Offsets[NR_BUFFERS];
			unsigned m_Counts[NR_BUFFERS];
			unsigned m_Capacities[NR_BUFFERS];
		};

		template <typename Vertex>
		InstanceArray::InstanceArray(const Vertex* pVertices, unsigned nrVertices, unsigned instanceStride, unsigned instancesCapacity, bool verticesImmutable, bool instancesImmutable)
			: m_Strides{ sizeof(Vertex), instanceStride }
			, m_Offsets{ 0,0 }
			, m_Counts{ nrVertices, 0 }
			, m_Capacities{ nrVertices, instancesCapacity }
		{
			Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_VERTICES], pVertices, nrVertices, verticesImmutable);
			Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_INSTANCES], instanceStride, instancesCapacity, instancesImmutable);;
		}

		template <typename Vertex, typename Instance>
		InstanceArray::InstanceArray(const Vertex* pVertices, unsigned nrVertices, const Instance* pInstances, unsigned nrInstances,
			bool verticesImmutable, bool instancesImmutable)
			: m_Strides{ sizeof(Vertex), sizeof(Instance) }
			, m_Offsets{ 0,0 }
			, m_Counts{ nrVertices, nrInstances }
			, m_Capacities{ nrVertices, nrInstances }
		{
			Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_VERTICES], pVertices, nrVertices, verticesImmutable);
			Dx::DxHelper::CreateInstanceBuffer(m_pBuffers[IDX_INSTANCES], pInstances, nrInstances, instancesImmutable);
		}

		template <typename Vertex>
		void InstanceArray::SetVertices(const Vertex* pVertices, unsigned nrVertices, bool immutable)
		{
			if (m_pBuffers[IDX_VERTICES]) m_pBuffers[IDX_VERTICES]->Release();
			Dx::DxHelper::CreateVertexBuffer(m_pBuffers[IDX_VERTICES], pVertices, nrVertices, immutable);
			m_Counts[IDX_VERTICES] = nrVertices;
		}

		template <typename Instance>
		void InstanceArray::SetInstances(const Instance* pInstances, unsigned nrInstances, bool immutable)
		{
			if (m_pBuffers[IDX_INSTANCES]) m_pBuffers[IDX_INSTANCES]->Release();
			Dx::DxHelper::CreateInstanceBuffer(m_pBuffers[IDX_INSTANCES], pInstances, nrInstances, immutable);
			m_Counts[IDX_INSTANCES] = nrInstances;
		}

		template <typename Instance>
		void InstanceArray::UpdateInstances(const Instance* pInstances, unsigned nrInstances)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			const HRESULT result{ Globals::pGpu->GetContext().Map(m_pBuffers[IDX_INSTANCES], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) };
			if (FAILED(result)) Logger::PrintError("Failed updating instances in InstanceArray");
			memcpy(mappedResource.pData, pInstances, nrInstances * sizeof(Instance));
			Globals::pGpu->GetContext().Unmap(m_pBuffers[IDX_INSTANCES], 0);
		}
	}
}
