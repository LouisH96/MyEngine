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
			explicit ConstantBuffer(const Gpu& gpu);
			ConstantBuffer(const Gpu& gpu, const Data& initData);
			~ConstantBuffer();
			ConstantBuffer(const ConstantBuffer& other) = delete;
			ConstantBuffer(ConstantBuffer&& other) noexcept = delete;
			ConstantBuffer& operator=(const ConstantBuffer& other) = delete;
			ConstantBuffer& operator=(ConstantBuffer&& other) noexcept = delete;

			void ActivateVs(const Gpu& gpu, int index = 0) const;
			void ActivatePs(const Gpu& gpu, int index = 0) const;
			void Activate(const Gpu& gpu, int index = 0) const;
			void Update(const Gpu& gpu, const Data& newData) const;

		private:
			ID3D11Buffer* m_pBuffer{};
		};

		template <typename Data>
		ConstantBuffer<Data>::ConstantBuffer(const Gpu& gpu)
		{
			Dx::DxHelper::CreateDynamicConstantBuffer<Data>(gpu.GetDevice(), m_pBuffer);
		}

		template <typename Data>
		ConstantBuffer<Data>::ConstantBuffer(const Gpu& gpu, const Data& initData)
		{
			Dx::DxHelper::CreateDynamicConstantBuffer<Data>(gpu.GetDevice(), m_pBuffer, initData);
		}

		template <typename Data>
		ConstantBuffer<Data>::~ConstantBuffer()
		{
			SAFE_RELEASE(m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::ActivateVs(const Gpu& gpu, int index) const
		{
			gpu.GetContext().VSSetConstantBuffers(index, 1, &m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::ActivatePs(const Gpu& gpu, int index) const
		{
			gpu.GetContext().PSSetConstantBuffers(index, 1, &m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::Activate(const Gpu& gpu, int index) const
		{
			gpu.GetContext().VSSetConstantBuffers(index, 1, &m_pBuffer);
			gpu.GetContext().PSSetConstantBuffers(index, 1, &m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::Update(const Gpu& gpu, const Data& newData) const
		{
			Dx::DxHelper::UpdateBuffer(gpu.GetContext(), *m_pBuffer, newData);
		}
	}
}

