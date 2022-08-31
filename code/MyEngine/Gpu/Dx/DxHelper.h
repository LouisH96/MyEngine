#pragma once
#include <d3d11.h>
#include <string>
#include <vector>

#define SAFE_RELEASE(x) if(x) { (x)->Release(); (x) = nullptr; }

struct ID3D11Device;
struct ID3D11VertexShader;
struct ID3D11PixelShader;

namespace MyEngine
{
	namespace Gpu
	{
		namespace Dx
		{
			class DxHelper
			{
			public:
				static void CreateVertexShader(ID3D11Device& device, const std::wstring& path, const std::string& functionName, ID3D11VertexShader*& pVertexShader);
				static void CreatePixelShader(ID3D11Device& device, const std::wstring& path, const std::string& functionName, ID3D11PixelShader*& pVertexShader);
				static void CreateComputeShader(ID3D11Device& device, const std::wstring& path, const std::string& functionName, ID3D11ComputeShader*& pShader);
				static void CompileFromFile(const std::wstring& path, const std::string& functionName, ID3DBlob*& pBlob);

				static void CreateVertexBufferView(ID3D11Device& device, ID3D11Buffer*& pVertexBuffer, ID3D11UnorderedAccessView*& pView);

				template <typename T>
				static void CreateDynamicConstantBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer, T* pInitData = nullptr);
				template <typename T>
				static void CreateImmutableConstantBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer, T& initData = nullptr);

				template <typename T>
				static void CreateRwStructuredBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer, ID3D11UnorderedAccessView*& pView, T* pInitData, size_t nrInitElems);
				template <typename T>
				static void CreateRwStructuredBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer, ID3D11UnorderedAccessView*& pView, const std::vector<T>& initData);

				template <typename T>
				static void CreateStructuredBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer, ID3D11ShaderResourceView*& pView, T* pInitData, size_t nrInitElems);
				template <typename T>
				static void CreateStructuredBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer, ID3D11ShaderResourceView*& pView, const std::vector<T>& initData);

			};

			template <typename T>
			void DxHelper::CreateDynamicConstantBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer, T* pInitData)
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

				const HRESULT result = device.CreateBuffer(&desc, pDataResource, &pBuffer);
				if (FAILED(result))
					throw std::exception("DxHelper::CreateDynamicConstantBuffer");
			}

			template <typename T>
			void DxHelper::CreateImmutableConstantBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer, T& initData)
			{
				const D3D11_BUFFER_DESC desc
				{
					sizeof(T), D3D11_USAGE_IMMUTABLE, D3D11_BIND_CONSTANT_BUFFER,
					0, 0, 0
				};
				const D3D11_SUBRESOURCE_DATA dataResource{ &initData };
				const HRESULT result = device.CreateBuffer(&desc, &dataResource, &pBuffer);
				if (FAILED(result))
					throw std::exception("DxHelper::CreateImmutableConstantBuffer");
			}

			template <typename T>
			void DxHelper::CreateRwStructuredBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer,
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

				HRESULT result = device.CreateBuffer(&bufferDesc, &resource, &pBuffer);
				if (FAILED(result))
					throw std::exception("DxHelper::CreateStructuredBuffer - Buffer");

				result = device.CreateUnorderedAccessView(pBuffer, &viewDesc, &pView);
				if (FAILED(result))
					throw std::exception("DxHelper::CreateStructuredBuffer - View");
			}

			template <typename T>
			void DxHelper::CreateRwStructuredBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer,
				ID3D11UnorderedAccessView*& pView, const std::vector<T>& initData)
			{
				CreateRwStructuredBuffer(device, pBuffer, pView, initData.data(), initData.size());
			}

			template <typename T>
			void DxHelper::CreateStructuredBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer,
				ID3D11ShaderResourceView*& pView, const std::vector<T>& initData)
			{
				CreateStructuredBuffer(device, pBuffer, pView, initData.data(), initData.size());
			}

			template <typename T>
			void DxHelper::CreateStructuredBuffer(ID3D11Device& device, ID3D11Buffer*& pBuffer,
				ID3D11ShaderResourceView*& pView, T* pInitData, size_t nrInitElems)
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

				HRESULT result = device.CreateBuffer(&bufferDesc, &resource, &pBuffer);
				if (FAILED(result))
					throw std::exception("DxHelper::CreateRwStructuredBuffer - Buffer");

				result = device.CreateShaderResourceView(pBuffer, &viewDesc, &pView);
				if (FAILED(result))
					throw std::exception("DxHelper::CreateRwStructuredBuffer - View");
			}
		}
	}
}
