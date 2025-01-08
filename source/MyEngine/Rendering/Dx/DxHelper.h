#pragma once

#include <d3d11.h>
#include <string>
#include <vector>

#include "Rendering/Gpu.h"
#include <DataStructures/Array.h>
#include <DataStructures/List.h>

#define SAFE_RELEASE(x){if(x){(x)->Release();(x)=nullptr;}}

struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace MyEngine
{
	namespace Rendering
	{
		namespace Dx
		{
			class DxHelper
			{
			public:
				static void CreateVertexShader(const std::wstring& path, const std::string& functionName, ID3D11VertexShader*& pVertexShader);
				static void CreatePixelShader(const std::wstring& path, const std::string& functionName, ID3D11PixelShader*& pVertexShader);
				static void CreateComputeShader(const std::wstring& path, const std::string& functionName, ID3D11ComputeShader*& pShader);
				static void CompileFromFile(const std::wstring& path, const std::string& functionName, ID3DBlob*& pBlob);

				static void CreateVertexBufferView(ID3D11Buffer*& pVertexBuffer, ID3D11UnorderedAccessView*& pView);
				static void CreateIndexBuffer(ID3D11Buffer*& pIndexBuffer, unsigned nrIndices, bool immutable);
				static void CreateIndexBuffer(ID3D11Buffer*& pIndexBuffer, const int* pInitIndices, int nrInitIndices);
				static void CreateIndexBuffer(ID3D11Buffer*& pIndexBuffer, const int* pIndices, int nrIndices, bool immutable);
				static void CreateIndexBuffer(ID3D11Buffer*& pIndexBuffer, const Array<int>& indices, bool immutable);

				template <typename T>
				static void CreateDynamicConstantBuffer(ID3D11Buffer*& pBuffer, T* pInitData = nullptr);
				template <typename T>
				static void CreateDynamicConstantBuffer(ID3D11Buffer*& pBuffer, unsigned count, T* pInitData = nullptr);
				template <typename T>
				static void CreateImmutableConstantBuffer(ID3D11Buffer*& pBuffer, T& initData = nullptr);

				template <typename T>
				static void UpdateBuffer(ID3D11Buffer& buffer, const T& bufferContent);
				template <typename T>
				static void UpdateBuffer(ID3D11Buffer*& buffer, const T* pData, unsigned count);
				template<typename T>
				static T* StartUpdateBuffer(ID3D11Buffer*& buffer);
				static void EndUpdateBuffer(ID3D11Buffer*& buffer);

				template <typename T>
				static void CreateRwStructuredBuffer(ID3D11Buffer*& pBuffer, ID3D11UnorderedAccessView*& pView, T* pInitData, size_t nrInitElems);
				template <typename T>
				static void CreateRwStructuredBuffer(ID3D11Buffer*& pBuffer, ID3D11UnorderedAccessView*& pView, const std::vector<T>& initData);

				template <typename T>
				static void CreateStructuredBuffer(ID3D11Buffer*& pBuffer, ID3D11ShaderResourceView*& pView, T* pInitData, size_t nrInitElems);
				template <typename T>
				static void CreateStructuredBuffer(ID3D11Buffer*& pBuffer, ID3D11ShaderResourceView*& pView, const std::vector<T>& initData);

				static void CreateVertexBuffer(ID3D11Buffer*& pBuffer, unsigned stride, int length, bool immutable);
				template<typename T>
				static void CreateVertexBuffer(ID3D11Buffer*& pBuffer, int length, bool immutable);
				template<typename T>
				static void CreateVertexBuffer(ID3D11Buffer*& pBuffer, const T* pData, int length, bool immutable);
				template<typename T>
				static void CreateVertexBuffer(ID3D11Buffer*& pBuffer, const Array<T>& data, bool immutable);
				template<typename T>
				static void CreateVertexBuffer(ID3D11Buffer*& pBuffer, const List<T>& data, bool immutable);
				template<typename T>
				static void CreateInstanceBuffer(ID3D11Buffer*& pBuffer, const T* pData, int length, bool immutable);
				template<typename T>
				static void CreateInstanceBuffer(ID3D11Buffer*& pBuffer, const Array<T>& data, bool immutable);

				static std::string GetHResultString(const HRESULT& result);

			private:
				static void AssertMultipleOf16(unsigned sizeOf);
			};

			template <typename T>
			void DxHelper::CreateDynamicConstantBuffer(ID3D11Buffer*& pBuffer, T* pInitData)
			{
				const D3D11_BUFFER_DESC desc
				{
					sizeof(T), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER,
					D3D11_CPU_ACCESS_WRITE, 0, 0
				};
				const D3D11_SUBRESOURCE_DATA dataResource{ pInitData };
				const D3D11_SUBRESOURCE_DATA* pDataResource{ pInitData
					? &dataResource
					: nullptr };

				const HRESULT result = Globals::pGpu->GetDevice().CreateBuffer(&desc, pDataResource, &pBuffer);
				if (FAILED(result))
				{
					Logger::PrintError("[DxHelper::CreateDynamicConstantBuffer]", result);
					AssertMultipleOf16(sizeof(T));
				}
			}
			template<typename T>
			inline void DxHelper::CreateDynamicConstantBuffer(ID3D11Buffer*& pBuffer, unsigned count, T* pInitData)
			{
				const D3D11_BUFFER_DESC desc
				{
					sizeof(T) * count, D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER,
					D3D11_CPU_ACCESS_WRITE, 0, 0
				};

				const D3D11_SUBRESOURCE_DATA dataResource{ pInitData };
				const D3D11_SUBRESOURCE_DATA* pDataResource{ pInitData
					? &dataResource
					: nullptr };
				const HRESULT result{
					Globals::pGpu->GetDevice().CreateBuffer(&desc, pDataResource, &pBuffer)
				};
				if (FAILED(result))
				{
					Logger::PrintError("[DxHelper::CreateDynamicConstantBuffer]", result);
					AssertMultipleOf16(sizeof(T));
				}
			}

			template <typename T>
			void DxHelper::CreateImmutableConstantBuffer(ID3D11Buffer*& pBuffer, T& initData)
			{
				const D3D11_BUFFER_DESC desc
				{
					sizeof(T), D3D11_USAGE_IMMUTABLE, D3D11_BIND_CONSTANT_BUFFER,
					0, 0, 0
				};
				const D3D11_SUBRESOURCE_DATA dataResource{ &initData };
				const HRESULT result = Globals::pGpu->GetDevice().CreateBuffer(&desc, &dataResource, &pBuffer);
				if (FAILED(result))
					Logger::PrintError("DxHelper::CreateImmutableConstantBuffer");
			}

			template <typename T>
			void DxHelper::UpdateBuffer(ID3D11Buffer& buffer, const T& bufferContent)
			{
				D3D11_MAPPED_SUBRESOURCE mappedResource{};
				Globals::pGpu->GetContext().Map(&buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				T& mappedContent = *static_cast<T*>(mappedResource.pData);
				mappedContent = bufferContent;
				Globals::pGpu->GetContext().Unmap(&buffer, 0);
			}

			template <typename T>
			void DxHelper::UpdateBuffer(ID3D11Buffer*& buffer, const T* pData, unsigned count)
			{
				D3D11_MAPPED_SUBRESOURCE mapped{};
				const HRESULT result{ Globals::pGpu->GetContext().Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped) };
				if (FAILED(result)) Logger::PrintError("[DxHelper::UpdateBuffer] failed updating buffer");
				std::copy(pData, &pData[count], static_cast<T*>(mapped.pData));
				Globals::pGpu->GetContext().Unmap(buffer, 0);
			}

			template <typename T>
			T* DxHelper::StartUpdateBuffer(ID3D11Buffer*& buffer)
			{
				D3D11_MAPPED_SUBRESOURCE resource{};
				const HRESULT result{ GPU.GetContext().Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource) };
				if (FAILED(result))
					Logger::PrintError("[DxHelper::StartUpdateBuffer] failed mapping buffer");
				return static_cast<T*>(resource.pData);
			}

			template <typename T>
			void DxHelper::CreateRwStructuredBuffer(ID3D11Buffer*& pBuffer,
				ID3D11UnorderedAccessView*& pView, T* pInitData, size_t nrInitElems)
			{
				const D3D11_BUFFER_DESC bufferDesc
				{
					sizeof(T) * static_cast<UINT>(nrInitElems), D3D11_USAGE_DEFAULT,
					D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
					0, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, sizeof(T)
				};
				const D3D11_UNORDERED_ACCESS_VIEW_DESC viewDesc
				{
					DXGI_FORMAT_UNKNOWN, D3D11_UAV_DIMENSION_BUFFER,
					D3D11_BUFFER_UAV{
						0, static_cast<UINT>(nrInitElems)
					}
				};
				const D3D11_SUBRESOURCE_DATA resource{ pInitData };

				HRESULT result = Globals::pGpu->GetDevice().CreateBuffer(&bufferDesc, &resource, &pBuffer);
				if (FAILED(result))
					Logger::PrintError("DxHelper::CreateStructuredBuffer - Buffer");

				result = Globals::pGpu->GetDevice().CreateUnorderedAccessView(pBuffer, &viewDesc, &pView);
				if (FAILED(result))
					Logger::PrintError("DxHelper::CreateStructuredBuffer - View");
			}

			template <typename T>
			void DxHelper::CreateRwStructuredBuffer(ID3D11Buffer*& pBuffer,
				ID3D11UnorderedAccessView*& pView, const std::vector<T>& initData)
			{
				CreateRwStructuredBuffer(Globals::pGpu->GetDevice(), pBuffer, pView, initData.data(), initData.size());
			}

			template <typename T>
			void DxHelper::CreateStructuredBuffer(ID3D11Buffer*& pBuffer,
				ID3D11ShaderResourceView*& pView, const std::vector<T>& initData)
			{
				CreateStructuredBuffer(Globals::pGpu->GetDevice(), pBuffer, pView, initData.data(), initData.size());
			}

			template <typename T>
			void DxHelper::CreateVertexBuffer(ID3D11Buffer*& pBuffer, int length, bool immutable)
			{
				const D3D11_BUFFER_DESC desc
				{
					sizeof(T) * length,
					immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC,
					D3D11_BIND_VERTEX_BUFFER,
					immutable ? UINT{0} : D3D11_CPU_ACCESS_WRITE,
					0, sizeof(T)
				};
				const HRESULT result{ Globals::pGpu->GetDevice().CreateBuffer(&desc, nullptr, &pBuffer) };
				if (FAILED(result))
					Logger::PrintError("Failed creating Vertex-Buffer");
			}

			template <typename T>
			void DxHelper::CreateVertexBuffer(ID3D11Buffer*& pBuffer, const T* pData, int length,
				bool immutable)
			{
				const D3D11_BUFFER_DESC desc
				{
					sizeof(T) * length,
					immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC,
					D3D11_BIND_VERTEX_BUFFER,
					immutable ? UINT{0} : D3D11_CPU_ACCESS_WRITE,
					0, sizeof(T)
				};
				const D3D11_SUBRESOURCE_DATA initData{ pData,0,0 };
				const HRESULT result{ Globals::pGpu->GetDevice().CreateBuffer(&desc, &initData, &pBuffer) };
				if (FAILED(result))
					Logger::PrintError("Failed creating Vertex-Buffer");
			}

			template <typename T>
			void DxHelper::CreateVertexBuffer(ID3D11Buffer*& pBuffer, const List<T>& data, bool immutable)
			{
				CreateVertexBuffer(pBuffer, data.GetData(), data.GetSize(), immutable);
			}

			template <typename T>
			void DxHelper::CreateVertexBuffer(ID3D11Buffer*& pBuffer, const Array<T>& data,
				bool immutable)
			{
				CreateVertexBuffer(pBuffer, data.GetData(), data.GetSize(), immutable);
			}

			template <typename T>
			void DxHelper::CreateInstanceBuffer(ID3D11Buffer*& pBuffer, const T* pData, int length,
				bool immutable)
			{
				CreateVertexBuffer(pBuffer, pData, length, immutable);
			}

			template <typename T>
			void DxHelper::CreateInstanceBuffer(ID3D11Buffer*& pBuffer, const Array<T>& data,
				bool immutable)
			{
				CreateInstanceBuffer(pBuffer, data, immutable);
			}

			template <typename T>
			void DxHelper::CreateStructuredBuffer(ID3D11Buffer*& pBuffer, ID3D11ShaderResourceView*& pView, T* pInitData, size_t nrInitElems)
			{
				const D3D11_BUFFER_DESC bufferDesc
				{
					sizeof(T) * static_cast<UINT>(nrInitElems), D3D11_USAGE_DEFAULT,
					D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE,
					0, D3D11_RESOURCE_MISC_BUFFER_STRUCTURED, sizeof(T)
				};
				const D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc
				{
					DXGI_FORMAT_UNKNOWN, D3D11_SRV_DIMENSION_BUFFEREX,
					D3D11_BUFFER_SRV{
						0, static_cast<UINT>(nrInitElems)
					}
				};
				const D3D11_SUBRESOURCE_DATA resource{ pInitData };

				HRESULT result = Globals::pGpu->GetDevice().CreateBuffer(&bufferDesc, &resource, &pBuffer);
				if (FAILED(result))
					Logger::PrintError("DxHelper::CreateRwStructuredBuffer - Buffer");

				result = Globals::pGpu->GetDevice().CreateShaderResourceView(pBuffer, &viewDesc, &pView);
				if (FAILED(result))
					Logger::PrintError("DxHelper::CreateRwStructuredBuffer - View");
			}
		}
	}
}

template<class Interface>
inline void SafeRelease(
	Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();
		(*ppInterfaceToRelease) = NULL;
	}
}