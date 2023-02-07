#include "pch.h"
#include "DxHelper.h"

#include <d3d11.h>
#include <d3dcompiler.h>

void MyEngine::App::Wrappers::Gpu::DxHelper::CreateVertexShader(ID3D11Device& device, const std::wstring& path, const std::string& functionName,
	ID3D11VertexShader*& pVertexShader)
{
	ID3D10Blob* pBlob{};
	CompileFromFile(path, functionName, pBlob);

	const HRESULT hr = device.CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader);

	if (!SUCCEEDED(hr))
		throw std::exception("DxHelper::CreateVertexShader - Error");
}

void MyEngine::App::Wrappers::Gpu::DxHelper::CreatePixelShader(ID3D11Device& device, const std::wstring& path, const std::string& functionName,
	ID3D11PixelShader*& pVertexShader)
{
	ID3D10Blob* pBlob{};
	CompileFromFile(path, functionName, pBlob);

	const HRESULT hr = device.CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader);

	if (!SUCCEEDED(hr))
		throw std::exception("DxHelper::CreatePixelShader - Error");
}

void MyEngine::App::Wrappers::Gpu::DxHelper::CreateComputeShader(ID3D11Device& device, const std::wstring& path, const std::string& functionName,
	ID3D11ComputeShader*& pShader)
{
	ID3D10Blob* pBlob{};
	CompileFromFile(path, functionName, pBlob);

	const HRESULT hr = device.CreateComputeShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pShader);

	if (!SUCCEEDED(hr))
		throw std::exception("DxHelper::CreateComputeShader - Error");
}

void MyEngine::App::Wrappers::Gpu::DxHelper::CompileFromFile(const std::wstring& path, const std::string& functionName, ID3DBlob*& pBlob)
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags
		|= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL; // add more debug output
#endif

	std::string target;
	if (functionName.substr(0, 1) == "v" || functionName.substr(0, 1) == "V")
		target = "vs_5_0";
	else if (functionName.substr(0, 1) == "p" || functionName.substr(0, 1) == "P")
		target = "ps_5_0";
	else
		target = "cs_5_0";

	ID3DBlob* pErrorBlob{};

	const HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		functionName.c_str(),
		target.c_str(),
		flags,
		0,
		&pBlob,
		&pErrorBlob);

	if (FAILED(hr)) {
		std::string error = "DxHelper::CompileFromFile - " + functionName;
		if (pErrorBlob)
		{
			error = static_cast<char*>(pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		if (pBlob)
			pBlob->Release();
		throw std::exception{ error.c_str() };
	}
}

void MyEngine::App::Wrappers::Gpu::DxHelper::CreateVertexBufferView(ID3D11Device& device, ID3D11Buffer*& pVertexBuffer,
	ID3D11UnorderedAccessView*& pView)
{
	D3D11_BUFFER_DESC descBuf = {};
	pVertexBuffer->GetDesc(&descBuf);
	descBuf.Usage = D3D11_USAGE_DEFAULT;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
	uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	uavDesc.Buffer.NumElements = descBuf.ByteWidth / 4;

	device.CreateUnorderedAccessView(pVertexBuffer, &uavDesc, &pView);
}

void MyEngine::App::Wrappers::Gpu::DxHelper::CreateIndexBuffer(ID3D11Device& device, ID3D11Buffer*& pIndexBuffer,
	const int* pInitIndices, int nrInitIndices)
{
	D3D11_BUFFER_DESC desc{};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(int) * nrInitIndices;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = pInitIndices;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	const HRESULT result = device.CreateBuffer(&desc, &initData, &pIndexBuffer);
	if (FAILED(result))
		Logger::PrintError("CreateIndexBuffer");
}
