#pragma once
#include <d3d11.h>

#include "../Dx/DxHelper.h"
#include "../Gpu.h"

namespace MyEngine
{
	namespace Rendering
	{
		template<typename Data>
		class ConstantBuffer
		{
		public:
			explicit ConstantBuffer();
			explicit ConstantBuffer(const Data& initData);
			explicit ConstantBuffer(unsigned size); //when array of Data
			~ConstantBuffer();
			ConstantBuffer(const ConstantBuffer& other) = delete;
			ConstantBuffer(ConstantBuffer&& other) noexcept = delete;
			ConstantBuffer& operator=(const ConstantBuffer& other) = delete;
			ConstantBuffer& operator=(ConstantBuffer&& other) noexcept = delete;

			void ActivateVs(int index = 0) const;
			void ActivatePs(int index = 0) const;
			void Activate(int index = 0) const;
			void Update(const Data& newData);
			void Update(const Data* pNewData, unsigned count);
			void Update(const Array<Data>& data);

			Data* StartUpdate();
			void EndUpdate();

		private:
			ID3D11Buffer* m_pBuffer{};
		};

		template <typename Data>
		ConstantBuffer<Data>::ConstantBuffer()
		{
			Dx::DxHelper::CreateDynamicConstantBuffer<Data>(m_pBuffer);
		}

		template <typename Data>
		ConstantBuffer<Data>::ConstantBuffer(const Data& initData)
		{
			Dx::DxHelper::CreateDynamicConstantBuffer<const Data>(m_pBuffer, &initData);
		}

		template<typename Data>
		inline ConstantBuffer<Data>::ConstantBuffer(unsigned size)
		{
			Dx::DxHelper::CreateDynamicConstantBuffer<const Data>(m_pBuffer, size);
		}

		template <typename Data>
		ConstantBuffer<Data>::~ConstantBuffer()
		{
			SAFE_RELEASE(m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::ActivateVs(int index) const
		{
			Globals::pGpu->GetContext().VSSetConstantBuffers(index, 1, &m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::ActivatePs(int index) const
		{
			Globals::pGpu->GetContext().PSSetConstantBuffers(index, 1, &m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::Activate(int index) const
		{
			Globals::pGpu->GetContext().VSSetConstantBuffers(index, 1, &m_pBuffer);
			Globals::pGpu->GetContext().PSSetConstantBuffers(index, 1, &m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::Update(const Data& newData)
		{
			Dx::DxHelper::UpdateBuffer(*m_pBuffer, newData);
		}

		template <typename Data>
		void ConstantBuffer<Data>::Update(const Data* pNewData, unsigned count)
		{
			Dx::DxHelper::UpdateBuffer(m_pBuffer, pNewData, count);
		}

		template<typename Data>
		void ConstantBuffer<Data>::Update(const Array<Data>& data)
		{
			Update(data.GetData(), data.GetSize());
		}
		template<typename Data>
		inline Data* ConstantBuffer<Data>::StartUpdate()
		{
			return Dx::DxHelper::StartUpdateBuffer<Data>(m_pBuffer);
		}
		template<typename Data>
		inline void ConstantBuffer<Data>::EndUpdate()
		{
			Dx::DxHelper::EndUpdateBuffer(m_pBuffer);
		}
	}
}

