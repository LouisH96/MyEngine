#pragma once
#include "../State/ConstantBuffer.h"
#include "IDrawData.h"

namespace MyEngine
{
	namespace Rendering
	{
		//VertexBuffer & InstanceBuffer (No IndexBuffer)
		class InstanceArray final : public IDrawData
		{
		public:
			//---| Constructor/Destructor |---
			InstanceArray() = default;
			template<typename Vertex>
			InstanceArray(const Vertex* pVertices, unsigned nrVertices, unsigned instanceStride, bool verticesImmutable = true);
			template<typename Vertex, typename Instance>
			InstanceArray(const Vertex* pVertices, unsigned nrVertices, const Instance* pInstances, unsigned nrInstances, bool verticesImmutable = true, bool instancesImmutable = true);
			template<typename Vertex, typename Instance>
			InstanceArray(Gpu& gpu, const Ds::Array<Vertex>& vertices, const Ds::Array<Instance>& instances, bool verticesImmutable = true, bool instancesImmutable = true);
			~InstanceArray() override;

			//---| Copy/Move |---
			InstanceArray(const InstanceArray& other) = delete;
			InstanceArray& operator=(const InstanceArray& other) = delete;
			InstanceArray(InstanceArray&& other) noexcept;
			InstanceArray& operator=(InstanceArray&& other) noexcept;

			//---| Functions |---
			void Draw(const Gpu& gpu) override;
			void Draw(unsigned instanceCount) const;

			unsigned GetVertexCount() const { return m_Counts[IDX_VERTICES]; }
			unsigned GetInstanceCount() const { return m_Counts[IDX_INSTANCES]; }

			template<typename Vertex> void SetVertices(const Vertex* pVertices, unsigned nrVertices, bool immutable = true);
			template<typename Instance> void SetInstances(const Instance* pInstances, unsigned nrInstances, bool immutable = true);

			template<typename Instance> void UpdateInstances(const Instance* pInstances);

		protected:
			static constexpr int NR_BUFFERS = 2;
			static constexpr int IDX_VERTICES = 0;
			static constexpr int IDX_INSTANCES = 1;
			ID3D11Buffer* m_pBuffers[NR_BUFFERS];
			unsigned int m_Strides[NR_BUFFERS];
			unsigned int m_Offsets[NR_BUFFERS];
			unsigned int m_Counts[NR_BUFFERS];
		};

		template <typename Vertex>
		InstanceArray::InstanceArray(const Vertex* pVertices, unsigned nrVertices, unsigned instanceStride, bool verticesImmutable)
			: m_Strides{ sizeof(Vertex), instanceStride }
			, m_Offsets{ 0,0 }
			, m_Counts{ nrVertices, 0 }
		{
			Dx::DxHelper::CreateVertexBuffer(*Globals::pGpu, m_pBuffers[IDX_VERTICES], pVertices, nrVertices, verticesImmutable);
			m_pBuffers[IDX_INSTANCES] = nullptr;
		}

		template <typename Vertex, typename Instance>
		InstanceArray::InstanceArray(const Vertex* pVertices, unsigned nrVertices, const Instance* pInstances, unsigned nrInstances,
			bool verticesImmutable, bool instancesImmutable)
			: m_Strides{ sizeof(Vertex), sizeof(Instance) }
			, m_Offsets{ 0,0 }
			, m_Counts{ nrVertices, nrInstances }
		{
			Dx::DxHelper::CreateVertexBuffer(*Globals::pGpu, m_pBuffers[IDX_VERTICES], pVertices, nrVertices, verticesImmutable);
			Dx::DxHelper::CreateInstanceBuffer(*Globals::pGpu, m_pBuffers[IDX_INSTANCES], pInstances, nrInstances, instancesImmutable);
		}

		template <typename Vertex, typename Instance>
		InstanceArray::InstanceArray(
			Gpu& gpu,
			const Array<Vertex>& vertices,
			const Array<Instance>& instances,
			bool verticesImmutable,
			bool instancesImmutable)
			: m_Strides{ sizeof(Vertex), sizeof(Instance) }
			, m_Offsets{ 0,0 }
			, m_Counts{ vertices.GetSize(), instances.GetSize() }
		{
			Dx::DxHelper::CreateVertexBuffer(gpu, m_pBuffers[IDX_VERTICES], vertices, verticesImmutable);
			Dx::DxHelper::CreateInstanceBuffer(gpu, m_pBuffers[IDX_INSTANCES], instances, instancesImmutable);
		}

		template <typename Vertex>
		void InstanceArray::SetVertices(const Vertex* pVertices, unsigned nrVertices, bool immutable)
		{
			if (m_pBuffers[IDX_VERTICES]) m_pBuffers[IDX_VERTICES]->Release();
			Dx::DxHelper::CreateVertexBuffer(*Globals::pGpu, m_pBuffers[IDX_VERTICES], pVertices, nrVertices, immutable);
			m_Counts[IDX_VERTICES] = nrVertices;
		}

		template <typename Instance>
		void InstanceArray::SetInstances(const Instance* pInstances, unsigned nrInstances, bool immutable)
		{
			if (m_pBuffers[IDX_INSTANCES]) m_pBuffers[IDX_INSTANCES]->Release();
			Dx::DxHelper::CreateInstanceBuffer(*Globals::pGpu, m_pBuffers[IDX_INSTANCES], pInstances, nrInstances, immutable);
			m_Counts[IDX_INSTANCES] = nrInstances;
		}

		template <typename Instance>
		void InstanceArray::UpdateInstances(const Instance* pInstances)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			const HRESULT result{ Globals::pGpu->GetContext().Map(m_pBuffers[IDX_INSTANCES], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) };
			if (FAILED(result)) Logger::PrintError("Failed updating instances in InstanceArray");
			memcpy(mappedResource.pData, pInstances, m_Counts[IDX_INSTANCES] * sizeof(Instance));
			Globals::pGpu->GetContext().Unmap(m_pBuffers[IDX_INSTANCES], 0);
		}
	}
}
