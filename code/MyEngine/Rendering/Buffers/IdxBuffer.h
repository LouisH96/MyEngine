#pragma once

struct ID3D11Buffer;

namespace MyEngine
{
	namespace Rendering
	{
		class IdxBuffer
		{
		public:
			//---| Construction |---
			IdxBuffer();
			explicit IdxBuffer(const int* pData, unsigned count);
			~IdxBuffer();

			IdxBuffer(const IdxBuffer& other) = delete;
			IdxBuffer& operator=(const IdxBuffer& other) = delete;
			IdxBuffer(IdxBuffer&& other) noexcept;
			IdxBuffer& operator=(IdxBuffer&& other) noexcept;

			//---| Functions |---
			void Activate() const;
			unsigned GetCapacity() const { return m_Capacity; }

			void Draw() const;

		private:
			ID3D11Buffer* m_pBuffer;
			unsigned m_Capacity;
		};
	}
}