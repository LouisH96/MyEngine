#pragma once

struct ID3D11Buffer;

namespace MyEngine
{
		class IdxBuffer
		{
		public:
			//---| Construction |---
			IdxBuffer();
			explicit IdxBuffer(unsigned capacity);
			explicit IdxBuffer(PtrRangeConst<int> indices, bool dynamic = false);
			explicit IdxBuffer(const int* pData, unsigned count, bool dynamic = false);
			~IdxBuffer();

			IdxBuffer(const IdxBuffer& other) = delete;
			IdxBuffer& operator=(const IdxBuffer& other) = delete;
			IdxBuffer(IdxBuffer&& other) noexcept;
			IdxBuffer& operator=(IdxBuffer&& other) noexcept;

			//---| Functions |---
			void Activate() const;
			unsigned GetCapacity() const { return m_Capacity; }
			void EnsureCapacityNoCopy(unsigned capacity, bool dynamic = false);

			void Draw() const;
			void Draw(unsigned start, unsigned count) const;
			void Draw(unsigned count) const;

			int* BeginCopyData();
			void EndCopyData();

		private:
			ID3D11Buffer* m_pBuffer;
			unsigned m_Capacity;
		};
}