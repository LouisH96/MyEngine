#include "pch.h"
#include "InputLayout.h"

#include <d3dcompiler.h>
#include <sstream>

#include "DxHelper.h"
#include "Gpu.h"

MyEngine::Dx::InputLayout::InputLayout(const App::Wrappers::Dx::Gpu& gpu, const Element* pElements, int nrElements)
{
	//CREATE INPUT_ELEMENT_DESC
	D3D11_INPUT_ELEMENT_DESC* pDxElements = new D3D11_INPUT_ELEMENT_DESC[nrElements];
	for(int i = 0; i < nrElements; i++)
	{
		D3D11_INPUT_ELEMENT_DESC& dxElement = pDxElements[i];
		const Element& element = pElements[i];

		dxElement.SemanticName = element.Semantic.c_str();
		dxElement.SemanticIndex = 0;
		dxElement.Format = ToDxFormat(element.Type);
		dxElement.InputSlot = 0;
		dxElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		dxElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		dxElement.InstanceDataStepRate = 0;
	}
	//CREATE COMPILED BLOB
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags
		|= D3DCOMPILE_DEBUG; // add more debug output
#endif
	const std::string entryPoint = "vs_main";
	const std::string target = "vs_5_0";
	const std::string dummyShaderText = CreateDummyShaderString(pElements, nrElements);
	ID3DBlob* pBlob{};
	ID3DBlob* pErrorBlob{};

	HRESULT result = D3DCompile(dummyShaderText.c_str(), dummyShaderText.size(),
		nullptr, nullptr, nullptr,
		entryPoint.c_str(), target.c_str(), flags,
		0, &pBlob, &pErrorBlob);
	if(FAILED(result))
	{
		std::string error = "CreateInputLayout-CompileDummy";
		if(pErrorBlob)
		{
			error = static_cast<char*>(pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		if (pBlob)
			pBlob->Release();
		Logger::PrintError(error);
	}

	//INPUTLAYOUT
	result = gpu.GetDevice().CreateInputLayout(
		pDxElements, nrElements,
		pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
		&m_pInputLayout);
	if (FAILED(result))
		Logger::PrintError("CreateInputLayout");

	SAFE_RELEASE(pBlob);
	SAFE_RELEASE(pErrorBlob);
	delete[] pDxElements;
}

MyEngine::Dx::InputLayout::~InputLayout()
{
	SAFE_RELEASE(m_pInputLayout);
}

void MyEngine::Dx::InputLayout::Activate(const App::Wrappers::Dx::Gpu& gpu) const
{
	gpu.GetContext().IASetInputLayout(m_pInputLayout);
}

DXGI_FORMAT MyEngine::Dx::InputLayout::ToDxFormat(ElementType type)
{
	switch(type)
	{
	case ElementType::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
	case ElementType::Float2: return DXGI_FORMAT_R32G32_FLOAT;
	default: return DXGI_FORMAT_UNKNOWN;
	}
}

std::string MyEngine::Dx::InputLayout::CreateDummyShaderString(const Element* pElements, int nrElements)
{
	std::stringstream ss;
	ss << "struct Vertex{\n";
	for(int i = 0; i < nrElements; i++)
	{
		const Element& element = pElements[i];
		ss << ToTypeString(element.Type) << " var" << i << ": " << element.Semantic << ";\n";
	}
	ss << "};\n";
	ss << "struct Pixel{float4 pos : VS_POSITION;};\n";
	ss << "Pixel vs_main(Vertex v){return (Pixel)0;}";
	return ss.str();
}

std::string MyEngine::Dx::InputLayout::ToTypeString(ElementType type)
{
	switch (type)
	{
	case ElementType::Float3: return "float3";
	case ElementType::Float2: return "float2";
	default: return "";
	}
}
