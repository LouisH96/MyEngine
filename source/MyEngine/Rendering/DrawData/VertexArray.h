#pragma once
#include <d3d11.h>

#include "../State/PrimitiveTopology.h"
#include "DataStructures/PtrRangeConst.h"
#include "Rendering/Dx/DxHelper.h"

namespace MyEngine
{
	namespace Rendering
	{
		//VertexArray & NO IndexBuffer & NO InstanceBuffer
		template<typename Vertex>
		class VertexArray
		{
		public:
			//---| Constructor/Destructor |---
			VertexArray();
			VertexArray(
				PtrRangeConst<Vertex>&& vertices,
				bool immutable = true,
				ModelTopology topology = ModelTopology::TriangleList);
			VertexArray(
				int stride, int initCount,
				bool immutable = true,
				ModelTopology topology = ModelTopology::TriangleList);
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
			void Draw(unsigned offset, unsigned count) const;

			unsigned int GetOffset() const { return m_Offset; }
			unsigned int GetCapacity() const { return m_Capacity; }
			unsigned int GetCount() const { return m_Count; }

			void SetOffset(unsigned int offset);
			void SetCapacity(unsigned int capacity, bool immutable);
			void SetCount(unsigned int count);

			void EnsureCapacity(unsigned int minCapacity, bool immutable);
			void UpdateData(const Vertex* pData, int dataCount);

			Vertex* BeginUpdateData();
			void EndUpdateData();

		private:
			ID3D11Buffer* m_pBuffer;
			unsigned int m_Stride;
			unsigned int m_Offset;
			unsigned int m_Capacity;
			unsigned int m_Count;
			D3D11_PRIMITIVE_TOPOLOGY m_Topology;
		};

		template <typename Vertex>
		VertexArray<Vertex>::VertexArray()
			: m_pBuffer{ nullptr }
			, m_Stride{ 0 }
			, m_Offset{ 0 }
			, m_Capacity{ 0 }
			, m_Count{ 0 }
			, m_Topology{ PrimitiveTopology::ToDx(ModelTopology::Unknown) }
		{
		}

		template <typename Vertex>
		VertexArray<Vertex>::VertexArray(
			PtrRangeConst<Vertex>&& vertices,
			bool immutable, ModelTopology topology)
			: m_pBuffer{ nullptr }
			, m_Stride{ sizeof(Vertex) }
			, m_Offset{ 0 }
			, m_Capacity{ vertices.count }
			, m_Count{ vertices.count }
			, m_Topology{ PrimitiveTopology::ToDx(topology) }
		{
			Dx::DxHelper::CreateVertexBuffer(m_pBuffer, vertices.pData, vertices.count, immutable);
		}

		template <typename Vertex>
		VertexArray<Vertex>::VertexArray(int stride, int initCount, bool immutable, ModelTopology topology)
			: m_pBuffer{ nullptr }
			, m_Stride{ static_cast<unsigned>(stride) }
			, m_Offset{ 0 }
			, m_Capacity{ static_cast<unsigned>(initCount) }
			, m_Count{ static_cast<unsigned>(initCount) }
			, m_Topology{ PrimitiveTopology::ToDx(topology) }
		{
			Dx::DxHelper::CreateVertexBuffer<char>(m_pBuffer, initCount * stride, immutable);
		}

		template <typename Vertex>
		VertexArray<Vertex>::~VertexArray()
		{
			if (m_pBuffer) m_pBuffer->Release();
			m_pBuffer = nullptr;
		}

		template <typename Vertex>
		VertexArray<Vertex>::VertexArray(VertexArray&& other) noexcept
			: m_pBuffer{ other.m_pBuffer }
			, m_Stride{ other.m_Stride }
			, m_Offset{ other.m_Offset }
			, m_Capacity{ other.m_Capacity }
			, m_Count{ other.m_Count }
			, m_Topology{ other.m_Topology }
		{
			other.m_pBuffer = nullptr;
		}

		template <typename Vertex>
		VertexArray<Vertex>& VertexArray<Vertex>::operator=(VertexArray&& other) noexcept
		{
			if (&other == this) return *this;
			m_pBuffer = other.m_pBuffer;
			m_Stride = other.m_Stride;
			m_Offset = other.m_Offset;
			m_Capacity = other.m_Capacity;
			m_Count = other.m_Count;
			m_Topology = other.m_Topology;
			other.m_pBuffer = nullptr;
			return *this;
		}

		template <typename Vertex>
		void VertexArray<Vertex>::Activate() const
		{
			Globals::pGpu->GetContext().IASetPrimitiveTopology(m_Topology);
			Globals::pGpu->GetContext().IASetVertexBuffers(0, 1, &m_pBuffer, &m_Stride, &m_Offset);
		}

		template <typename Vertex>
		void VertexArray<Vertex>::Activate(int idx) const
		{
			Globals::pGpu->GetContext().IASetPrimitiveTopology(m_Topology);
			Globals::pGpu->GetContext().IASetVertexBuffers(idx, 1, &m_pBuffer, &m_Stride, &m_Offset);
		}

		template <typename Vertex>
		void VertexArray<Vertex>::Draw() const
		{
			Globals::pGpu->GetContext().Draw(m_Count, 0);
		}

		template <typename Vertex>
		void VertexArray<Vertex>::Draw(unsigned offset, unsigned count) const
		{
			Globals::pGpu->GetContext().Draw(count, offset);
		}

		template <typename Vertex>
		void VertexArray<Vertex>::SetOffset(unsigned offset)
		{
			m_Offset = offset;
		}

		template <typename Vertex>
		void VertexArray<Vertex>::SetCapacity(unsigned capacity, bool immutable)
		{
			if (m_Capacity == capacity) return;
			m_pBuffer->Release();
			Dx::DxHelper::CreateVertexBuffer(m_pBuffer, m_Stride, capacity, immutable);
			m_Capacity = capacity;
		}

		template <typename Vertex>
		void VertexArray<Vertex>::SetCount(unsigned count)
		{
			m_Count = count;
		}

		template <typename Vertex>
		void VertexArray<Vertex>::EnsureCapacity(unsigned minCapacity, bool immutable)
		{
			if (m_Capacity >= minCapacity) return;
			SetCapacity(minCapacity, immutable);
		}

		template <typename Vertex>
		void VertexArray<Vertex>::UpdateData(const Vertex* pData, int dataCount)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			const HRESULT result{ Globals::pGpu->GetContext().Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) };
			if (FAILED(result)) Logger::PrintError("Failed updating VertexArray data");
			memcpy(mappedResource.pData, pData, dataCount * sizeof(Vertex));
			Globals::pGpu->GetContext().Unmap(m_pBuffer, 0);
		}

		template<typename Vertex>
		inline Vertex* VertexArray<Vertex>::BeginUpdateData()
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource{};
			const HRESULT result{ Globals::pGpu->GetContext().Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0 ,&mappedResource) };
			if (FAILED(result))
				Logger::PrintError("[VertexArray::BeginUpdateData] mapping failed");
			return reinterpret_cast<Vertex*>(mappedResource.pData);
		}
		template<typename Vertex>
		inline void VertexArray<Vertex>::EndUpdateData()
		{
			Globals::pGpu->GetContext().Unmap(m_pBuffer, 0);
		}
	}
}
