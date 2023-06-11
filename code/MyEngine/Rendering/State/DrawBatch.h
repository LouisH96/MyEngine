#pragma once
#include "ConstantBuffer.h"
#include "IDrawBatch.h"

namespace MyEngine
{
	namespace Rendering
	{
		//VertexBuffer & InstanceBuffer (No IndexBuffer)
		class DrawBatch final : public IDrawBatch
		{
		public:
			//---| Constructor/Destructor |---
			DrawBatch() = default;
			template<typename Vertex, typename Instance>
			DrawBatch(Vertex* pVertices, unsigned nrVertices, Instance* pInstances, unsigned nrInstances, bool verticesImmutable = true, bool instancesImmutable = true);
			template<typename Vertex, typename Instance>
			DrawBatch(Gpu& gpu, const Array<Vertex>& vertices, const Array<Instance>& instances, bool verticesImmutable = true, bool instancesImmutable = true);
			~DrawBatch() override;

			//---| Copy/Move |---
			DrawBatch(const DrawBatch& other) = delete;
			DrawBatch& operator=(const DrawBatch& other) = delete;
			DrawBatch(DrawBatch&& other) noexcept;
			DrawBatch& operator=(DrawBatch&& other) noexcept;

			//---| Functions |---
			void Draw(const Gpu& gpu) override;

			template<typename Vertex> void SetVertices(const Vertex* pVertices, unsigned nrVertices, bool immutable = true);
			template<typename Instance> void SetInstances(const Instance* pInstances, unsigned nrInstances, bool immutable = true);

		private:
			static constexpr int NR_BUFFERS = 2;
			static constexpr int IDX_VERTICES = 0;
			static constexpr int IDX_INSTANCES = 1;
			ID3D11Buffer* m_pBuffers[NR_BUFFERS];
			unsigned int m_Strides[NR_BUFFERS];
			unsigned int m_Offsets[NR_BUFFERS];
			unsigned int m_Counts[NR_BUFFERS];
		};

		template <typename Vertex, typename Instance>
		DrawBatch::DrawBatch(Vertex* pVertices, unsigned nrVertices, Instance* pInstances, unsigned nrInstances,
			bool verticesImmutable, bool instancesImmutable)
			: m_Strides{ sizeof(Vertex), sizeof(Instance) }
			, m_Offsets{ 0,0 }
			, m_Counts{ nrVertices, nrInstances }
		{
			if (nrVertices > 0) Dx::DxHelper::CreateVertexBuffer(*Globals::pGpu, m_pBuffers[IDX_VERTICES], pVertices, nrVertices, verticesImmutable);
			else m_pBuffers[IDX_VERTICES] = nullptr;

			if (nrInstances > 0) Dx::DxHelper::CreateInstanceBuffer(*Globals::pGpu, m_pBuffers[IDX_INSTANCES], pInstances, nrInstances, instancesImmutable);
			else m_pBuffers[IDX_INSTANCES] = nullptr;
		}

		template <typename Vertex, typename Instance>
		DrawBatch::DrawBatch(
			Gpu& gpu,
			const Array<Vertex>& vertices,
			const Array<Instance>& instances,
			bool verticesImmutable,
			bool instancesImmutable)
			: m_Strides{ sizeof(Vertex), sizeof(Instance) }
			, m_Offsets{ 0,0 }
			, m_Counts{ vertices.GetSize(), instances.GetSize() }
		{
			if (m_Counts[IDX_VERTICES] > 0) Dx::DxHelper::CreateVertexBuffer(gpu, m_pBuffers[IDX_VERTICES], vertices, verticesImmutable);
			else m_pBuffers[IDX_VERTICES] = nullptr;

			if (m_Counts[IDX_INSTANCES] > 0) Dx::DxHelper::CreateInstanceBuffer(gpu, m_pBuffers[IDX_INSTANCES], instances, instancesImmutable);
			else m_pBuffers[IDX_INSTANCES] = nullptr;
		}

		template <typename Vertex>
		void DrawBatch::SetVertices(const Vertex* pVertices, unsigned nrVertices, bool immutable)
		{
			if (m_pBuffers[IDX_VERTICES]) m_pBuffers[IDX_VERTICES]->Release();
			Dx::DxHelper::CreateVertexBuffer(*Globals::pGpu, m_pBuffers[IDX_VERTICES], pVertices, nrVertices, immutable);
			m_Counts[IDX_VERTICES] = nrVertices;
		}

		template <typename Instance>
		void DrawBatch::SetInstances(const Instance* pInstances, unsigned nrInstances, bool immutable)
		{
			if (m_pBuffers[IDX_INSTANCES]) m_pBuffers[IDX_INSTANCES]->Release();
			Dx::DxHelper::CreateInstanceBuffer(*Globals::pGpu, m_pBuffers[IDX_INSTANCES], pInstances, nrInstances, immutable);
			m_Counts[IDX_INSTANCES] = nrInstances;
		}
	}
}
