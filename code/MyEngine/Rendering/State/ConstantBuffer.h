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
			ConstantBuffer(const Data& initData);
			~ConstantBuffer();
			ConstantBuffer(const ConstantBuffer& other) = delete;
			ConstantBuffer(ConstantBuffer&& other) noexcept = delete;
			ConstantBuffer& operator=(const ConstantBuffer& other) = delete;
			ConstantBuffer& operator=(ConstantBuffer&& other) noexcept = delete;

			void ActivateVs(int index = 0) const;
			void ActivatePs(int index = 0) const;
			void Activate(int index = 0) const;
			void Update(const Data& newData) const;

		private:
			ID3D11Buffer* m_pBuffer{};
		};

		template <typename Data>
		ConstantBuffer<Data>::ConstantBuffer()
		{
			Dx::DxHelper::CreateDynamicConstantBuffer<Data>(Globals::pGpu->GetDevice(), m_pBuffer);
		}

		template <typename Data>
		ConstantBuffer<Data>::ConstantBuffer(const Data& initData)
		{
			Dx::DxHelper::CreateDynamicConstantBuffer<Data>(Globals::pGpu->GetDevice(), m_pBuffer, initData);
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
		void ConstantBuffer<Data>::Update(const Data& newData) const
		{
			Dx::DxHelper::UpdateBuffer(Globals::pGpu->GetContext(), *m_pBuffer, newData);
		}
	}
}

