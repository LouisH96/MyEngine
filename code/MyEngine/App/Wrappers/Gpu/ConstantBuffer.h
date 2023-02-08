#pragma once
#include <d3d11.h>

#include "DxHelper.h"
#include "App/Wrappers/Gpu/Gpu.h"

namespace MyEngine
{
	namespace Gpu
	{
		template<typename Data>
		class ConstantBuffer
		{
		public:
			explicit ConstantBuffer(const App::Wrappers::Gpu::Gpu& gpu);
			ConstantBuffer(const App::Wrappers::Gpu::Gpu& gpu, const Data& initData);
			~ConstantBuffer();
			ConstantBuffer(const ConstantBuffer& other) = delete;
			ConstantBuffer(ConstantBuffer&& other) noexcept = delete;
			ConstantBuffer& operator=(const ConstantBuffer& other) = delete;
			ConstantBuffer& operator=(ConstantBuffer&& other) noexcept = delete;

			void Activate(const App::Wrappers::Gpu::Gpu& gpu, int index = 0) const;
			void Update(const App::Wrappers::Gpu::Gpu& gpu, const Data& newData) const;

		private:
			ID3D11Buffer* m_pBuffer{};
		};

		template <typename Data>
		ConstantBuffer<Data>::ConstantBuffer(const App::Wrappers::Gpu::Gpu& gpu)
		{
			App::Wrappers::Gpu::DxHelper::CreateDynamicConstantBuffer<Data>(gpu.GetDevice(), m_pBuffer);
		}

		template <typename Data>
		ConstantBuffer<Data>::ConstantBuffer(const App::Wrappers::Gpu::Gpu& gpu, const Data& initData)
		{
			App::Wrappers::Gpu::DxHelper::CreateDynamicConstantBuffer<Data>(gpu.GetDevice(), m_pBuffer, initData);
		}

		template <typename Data>
		ConstantBuffer<Data>::~ConstantBuffer()
		{
			SAFE_RELEASE(m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::Activate(const App::Wrappers::Gpu::Gpu& gpu, int index) const
		{
			gpu.GetContext().VSSetConstantBuffers(index, 1, &m_pBuffer);
		}

		template <typename Data>
		void ConstantBuffer<Data>::Update(const App::Wrappers::Gpu::Gpu& gpu, const Data& newData) const
		{
			App::Wrappers::Gpu::DxHelper::UpdateBuffer(gpu.GetContext(), *m_pBuffer, newData);
		}
	}
}

