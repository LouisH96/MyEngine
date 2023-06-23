#pragma once
#include <d3d11.h>

#include "../State/PrimitiveTopology.h"
#include "Rendering/Dx/DxHelper.h"

namespace MyEngine
{
	namespace Rendering
	{
		class Gpu;

		//VertexArray & NO IndexBuffer & NO InstanceBuffer
		class VertexArray
		{
		public:
			//---| Constructor/Destructor |---
			VertexArray();
			template<typename Vertex>
			VertexArray(
				const Vertex* pInitData, int initSize,
				bool immutable = true,
				PrimitiveTopology topology = PrimitiveTopology::TriangleList);
			VertexArray(
				int stride, int initCount,
				bool immutable = true,
				PrimitiveTopology topology = PrimitiveTopology::TriangleList);
			~VertexArray();

			//---| Move/Copy |---
			VertexArray(const VertexArray& other) = delete;
			VertexArray& operator=(const VertexArray& other) = delete;
			VertexArray(VertexArray&& other) noexcept;
			VertexArray& operator=(VertexArray&& other) noexcept;

			//---| Functions |---
			void Activate() const;
			void Activate(int idx) const;
			void Draw() const;

			unsigned int GetOffset() const { return m_Offset; }
			unsigned int GetCapacity() const { return m_Capacity; }
			unsigned int GetCount() const { return m_Count; }

			void SetOffset(unsigned int offset);
			void SetCapacity(unsigned int capacity, bool immutable);
			void SetCount(unsigned int count);

			void EnsureCapacity(unsigned int minCapacity, bool immutable);
			template<typename T> void UpdateData(T* pData, int dataCount);

		private:
			ID3D11Buffer* m_pBuffer;
			unsigned int m_Stride;
			unsigned int m_Offset;
			unsigned int m_Capacity;
			unsigned int m_Count;
			D3D11_PRIMITIVE_TOPOLOGY m_Topology;
		};

		template <typename Vertex>
		VertexArray::VertexArray(
			const Vertex* pInitData, int initSize,
			bool immutable, PrimitiveTopology topology)
			: m_pBuffer{ nullptr }
			, m_Stride{ sizeof(Vertex) }
			, m_Offset{ 0 }
			, m_Capacity{ static_cast<unsigned>(initSize) }
			, m_Count{ static_cast<unsigned>(initSize) }
			, m_Topology{ PrimitiveTopologyUtils::ToDx(topology) }
		{
			Dx::DxHelper::CreateVertexBuffer(m_pBuffer, pInitData, initSize, immutable);
		}

		template <typename T>
		void VertexArray::UpdateData(T* pData, int dataCount)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			const HRESULT result{ Globals::pGpu->GetContext().Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) };
			if (FAILED(result)) Logger::PrintError("Failed updating VertexArray data");
			memcpy(mappedResource.pData, pData, dataCount * sizeof(T));
			Globals::pGpu->GetContext().Unmap(m_pBuffer, 0);
		}
	}
}
