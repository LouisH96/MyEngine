#pragma once
#include <d3d11.h>

#include "PrimitiveTopology.h"
#include "Rendering/Dx/DxHelper.h"

namespace MyEngine
{
	namespace Rendering
	{
		class Gpu;

		//VertexBuffer & NO IndexBuffer & NO InstanceBuffer
		class VertexBuffer
		{
		public:
			//---| Constructor/Destructor |---
			VertexBuffer() = default;
			template<typename Vertex>
			VertexBuffer(const Gpu& gpu,
				const Vertex* pInitData, int initSize,
				bool immutable = true,
				PrimitiveTopology topology = PrimitiveTopology::TriangleList);
			VertexBuffer(const Gpu& gpu,
				int stride, int initCount,
				bool immutable = true,
				PrimitiveTopology topology = PrimitiveTopology::TriangleList);
			~VertexBuffer();

			//---| Move/Copy |---
			VertexBuffer(const VertexBuffer& other) = delete;
			VertexBuffer& operator=(const VertexBuffer& other) = delete;
			VertexBuffer(VertexBuffer&& other) noexcept;
			VertexBuffer& operator=(VertexBuffer&& other) noexcept;

			//---| Functions |---
			void Activate(const Gpu& gpu) const;
			void Draw(const Gpu& gpu) const;

			unsigned int GetOffset() const { return m_Offset; }
			unsigned int GetCapacity() const { return m_Capacity; }
			unsigned int GetCount() const { return m_Count; }

			void SetOffset(unsigned int offset);
			void SetCapacity(const Gpu& gpu, unsigned int capacity, bool immutable);
			void SetCount(unsigned int count);

			void EnsureCapacity(const Gpu& gpu, unsigned int minCapacity, bool immutable);
			template<typename T> void UpdateData(const Gpu& gpu, T* pData, int dataCount);

		private:
			ID3D11Buffer* m_pBuffer;
			unsigned int m_Stride;
			unsigned int m_Offset;
			unsigned int m_Capacity;
			unsigned int m_Count;
			D3D11_PRIMITIVE_TOPOLOGY m_Topology;
		};

		template <typename Vertex>
		VertexBuffer::VertexBuffer(
			const Gpu& gpu,
			const Vertex* pInitData, int initSize,
			bool immutable, PrimitiveTopology topology)
			: m_pBuffer{ nullptr }
			, m_Stride{ sizeof(Vertex) }
			, m_Offset{ 0 }
			, m_Capacity{ static_cast<unsigned>(initSize) }
			, m_Count{ static_cast<unsigned>(initSize) }
			, m_Topology{ PrimitiveTopologyUtils::ToDx(topology) }
		{
			Dx::DxHelper::CreateVertexBuffer(gpu, m_pBuffer, pInitData, initSize, immutable);
		}

		template <typename T>
		void VertexBuffer::UpdateData(const Gpu& gpu, T* pData, int dataCount)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			const HRESULT result{ gpu.GetContext().Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) };
			if (FAILED(result)) Logger::PrintError("Failed updating VertexBuffer data");
			memcpy(mappedResource.pData, pData, dataCount * sizeof(T));
			gpu.GetContext().Unmap(m_pBuffer, 0);
		}
	}
}
