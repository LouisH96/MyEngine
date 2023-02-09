#pragma once
#include <d3d11.h>

#include "DxHelper.h"
#include "App/Wrappers/Dx/Gpu.h"

namespace MyEngine
{
	namespace Dx
	{
		template<typename Data>
		class ConstantBuffer
		{
		public:
			explicit ConstantBuffer(const App::Wrappers::Dx::Gpu& gpu);
			ConstantBuffer(const App::Wrappers::Dx::Gpu& gpu, const Data& initData);
			~ConstantBuffer();
			ConstantBuffer(const ConstantBuffer& other) = delete;
			ConstantBuffer(ConstantBuffer&& other) noexcept = delete;
			ConstantBuffer& operator=(const ConstantBuffer& other) = delete;
			ConstantBuffer& operator=(ConstantBuffer&& other) noexcept = delete;

			void ActivateVs(const App::Wrappers::Dx::Gpu& gpu, int index = 0) const;
			void ActivatePs(const App::Wrappers::Dx::Gpu& gpu, int index = 0) const;
			void Update(const App::Wrappers::Dx::Gpu& gpu, const Data& newData) const;

		private:
			ID3D11Buffer* m_pBuffer{};
		};

		template <typename Data>
		ConstantBuffer<Data>::ConstantBuffer(const App::Wrappers::Dx::Gpu& gpu)
		{
			App::Wrappers::Dx::DxHelper::CreateDynamicConstantBuffer<Data>(gpu.GetDevice(), m_pBuffer);
		}

		template <typename Data>
		ConstantBuffer<Data>::ConstantBuffer(const App::Wrappers::Dx::Gpu& gpu, const Data& initData)
		{
			App::Wrappers::Dx::DxHelper::CreateDynamicConstantBuffer<Data>(gpu.GetDevice(), m_pBuffer, initData);
		}

		template <typename Data>
		ConstantBuffer<Data>::~ConstantBuffer()
		{
			SAFE_RELEASE(m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::ActivateVs(const App::Wrappers::Dx::Gpu& gpu, int index) const
		{
			gpu.GetContext().VSSetConstantBuffers(index, 1, &m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::ActivatePs(const App::Wrappers::Dx::Gpu& gpu, int index) const
		{
			gpu.GetContext().PSSetConstantBuffers(index, 1, &m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::Update(const App::Wrappers::Dx::Gpu& gpu, const Data& newData) const
		{
			App::Wrappers::Dx::DxHelper::UpdateBuffer(gpu.GetContext(), *m_pBuffer, newData);
		}
	}
}

