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
			Dx::DxHelper::CreateVertexBuffer(gpu, m_pBuffers[IDX_VERTICES], vertices, verticesImmutable);
			Dx::DxHelper::CreateInstanceBuffer(gpu, m_pBuffers[IDX_INSTANCES], instances, instancesImmutable);
		}
	}
}
