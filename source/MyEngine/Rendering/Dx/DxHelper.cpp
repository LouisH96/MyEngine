#include "pch.h"
#include "DxHelper.h"

#include <comdef.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "ShaderIncludes.h"
#include "String/Convert.h"

using namespace Rendering;
using namespace Dx;

void DxHelper::CreateVertexShader(const std::wstring& path, const std::string& functionName,
	ID3D11VertexShader*& pVertexShader)
{
	ID3D10Blob* pBlob{};
	CompileFromFile(path, functionName, pBlob);

	const HRESULT hr = Globals::pGpu->GetDevice().CreateVertexShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader);
	OnFail("[DxHelper::CreateVertexShader]", hr);
}

void DxHelper::CreatePixelShader(const std::wstring& path, const std::string& functionName,
	ID3D11PixelShader*& pVertexShader)
{
	ID3D10Blob* pBlob{};
	CompileFromFile(path, functionName, pBlob);

	const HRESULT hr = Globals::pGpu->GetDevice().CreatePixelShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pVertexShader);
	OnFail("[DxHelper::CreatePixelShader]", hr);
}

void DxHelper::CreateComputeShader(const std::wstring& path, const std::string& functionName,
	ID3D11ComputeShader*& pShader)
{
	ID3D10Blob* pBlob{};
	CompileFromFile(path, functionName, pBlob);

	const HRESULT hr = Globals::pGpu->GetDevice().CreateComputeShader(
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr,
		&pShader);
	OnFail("[DxHelper::CreateComputeShader]", hr);
}

void DxHelper::CompileFromFile(const std::wstring& path, const std::string& functionName, ID3DBlob*& pBlob)
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

	ShaderIncludes includes{ path };

	const HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		&includes,
		functionName.c_str(),
		target.c_str(),
		flags,
		0,
		&pBlob,
		&pErrorBlob);

	if (pErrorBlob)
	{
		std::stringstream ss{};
		ss << "[DxHelper::CompileFromFile]\n";
		ss << "\t[Path: " << Convert::ToString(path) << "]\n";
		ss << "\t[HResult: " << GetHResultString(hr) << "]\n";
		if (pErrorBlob)
		{
			ss << "\t" << static_cast<char*>(pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		if (OnFail(ss.str(), hr))
		{
			if (pBlob)
				pBlob->Release();
		}
		else
			Logger::Warning(ss.str());
	}
	else if (FAILED(hr))
	{
		std::stringstream ss{};
		ss << "[DxHelper::CompileFromFile]\n";
		ss << "\t[Path: " << Convert::ToString(path) << "]\n";
		ss << "\t[HResult: " << GetHResultString(hr) << "]\n";
		Logger::Error(ss.str());
		if (pBlob)
			pBlob->Release();
	}
}

void DxHelper::CreateVertexBufferView(ID3D11Buffer*& pVertexBuffer, ID3D11UnorderedAccessView*& pView)
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

	Globals::pGpu->GetDevice().CreateUnorderedAccessView(pVertexBuffer, &uavDesc, &pView);
}

void DxHelper::CreateIndexBuffer(ID3D11Buffer*& pIndexBuffer, unsigned nrIndices, bool immutable)
{
	D3D11_BUFFER_DESC desc{};
	desc.Usage = immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(int) * nrIndices;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = immutable ? 0 : D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	const HRESULT result{ Globals::pGpu->GetDevice().CreateBuffer(&desc, nullptr, &pIndexBuffer) };
	OnFail("[DxHelper::CreateIndexBuffer(count only)]", result);
}

void DxHelper::CreateIndexBuffer(ID3D11Buffer*& pIndexBuffer, const int* pInitIndices, int nrInitIndices)
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

	const HRESULT result = Globals::pGpu->GetDevice().CreateBuffer(&desc, &initData, &pIndexBuffer);
	OnFail("[DxHelper::CreateIndexBuffer]", result);
}

void DxHelper::CreateIndexBuffer(ID3D11Buffer*& pIndexBuffer, const int* pIndices, int nrIndices, bool immutable)
{
	D3D11_BUFFER_DESC desc{};
	desc.Usage = immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(int) * nrIndices;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = immutable ? 0 : D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	const D3D11_SUBRESOURCE_DATA initData{ pIndices,0,0 };

	const HRESULT result = Globals::pGpu->GetDevice().CreateBuffer(&desc, &initData, &pIndexBuffer);
	OnFail("[DxHelper::CreateIndexBuffer]", result);
}

void DxHelper::CreateIndexBuffer(ID3D11Buffer*& pIndexBuffer, const Array<int>& indices, bool immutable)
{
	CreateIndexBuffer(pIndexBuffer, indices.GetData(), indices.GetSize(), immutable);
}

void DxHelper::EndUpdateBuffer(ID3D11Buffer*& buffer)
{
	GPU.GetContext().Unmap(buffer, 0);
}

void DxHelper::CreateVertexBuffer(ID3D11Buffer*& pBuffer, unsigned stride, int length,
	bool immutable)
{
	const D3D11_BUFFER_DESC desc
	{
		stride * length,
		immutable ? D3D11_USAGE_IMMUTABLE : D3D11_USAGE_DYNAMIC,
		D3D11_BIND_VERTEX_BUFFER,
		immutable ? 0u : D3D11_CPU_ACCESS_WRITE,
		0, stride
	};
	const HRESULT result{ Globals::pGpu->GetDevice().CreateBuffer(&desc, nullptr, &pBuffer) };
	OnFail("[DxHelper::CreateVertexBuffer]", result);
}

std::string DxHelper::GetHResultString(const HRESULT& result)
{
	const _com_error error{ result };
	const std::wstring wString = { error.ErrorMessage() };
	return Convert::ToString(wString);
}

void DxHelper::LogError(const std::string& prefix, const HRESULT& result)
{
	Logger::Error(prefix, GetHResultString(result));
}

bool DxHelper::OnFail(const std::string& prefix, const HRESULT& result)
{
	if (FAILED(result))
	{
		LogError(prefix, result);
		return true;
	}
	return false;
}

void DxHelper::AssertMultipleOf16(unsigned sizeOf)
{
	if (sizeOf % 16 != 0)
		Logger::Warning("[DxHelper] buffer size should be multiple of 16");
}
