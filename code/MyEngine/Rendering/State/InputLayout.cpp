#include "pch.h"
#include "InputLayout.h"

#include <d3dcompiler.h>
#include <sstream>

#include "../Dx/DxHelper.h"
#include "../Gpu.h"

Rendering::InputLayout::InputLayout(const Element* pElements, int nrElements)
{
	//CREATE INPUT_ELEMENT_DESC
	D3D11_INPUT_ELEMENT_DESC* pDxElements = new D3D11_INPUT_ELEMENT_DESC[nrElements];
	for (int i = 0; i < nrElements; i++)
	{
		D3D11_INPUT_ELEMENT_DESC& dxElement = pDxElements[i];
		const Element& element = pElements[i];

		dxElement.SemanticName = element.Semantic.c_str();
		dxElement.SemanticIndex = 0;
		dxElement.Format = ToDxFormat(element.Type);
		dxElement.InputSlot = element.InputSlot;
		dxElement.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		dxElement.InputSlotClass = ToDx(element.SlotClass);
		dxElement.InstanceDataStepRate = element.SlotClass == SlotClass::PerInstance ? 1 : 0;
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
	if (FAILED(result))
	{
		std::string error = "CreateInputLayout-CompileDummy";
		if (pErrorBlob)
		{
			error = static_cast<char*>(pErrorBlob->GetBufferPointer());
			pErrorBlob->Release();
		}
		if (pBlob)
			pBlob->Release();
		Logger::PrintError(error);
	}

	//INPUTLAYOUT
	result = Globals::pGpu->GetDevice().CreateInputLayout(
		pDxElements, nrElements,
		pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
		&m_pInputLayout);
	if (FAILED(result))
		Logger::PrintError("CreateInputLayout");

	SAFE_RELEASE(pBlob);
	SAFE_RELEASE(pErrorBlob);
	delete[] pDxElements;
}

Rendering::InputLayout::~InputLayout()
{
	SAFE_RELEASE(m_pInputLayout);
}

Rendering::InputLayout::InputLayout(InputLayout&& other) noexcept
	: m_pInputLayout{ other.m_pInputLayout }
{
	other.m_pInputLayout = nullptr;
}

Rendering::InputLayout& Rendering::InputLayout::operator=(InputLayout&& other) noexcept
{
	if (&other == this) return *this;
	m_pInputLayout = other.m_pInputLayout;
	other.m_pInputLayout = nullptr;
	return *this;
}

void Rendering::InputLayout::Activate() const
{
	Globals::pGpu->GetContext().IASetInputLayout(m_pInputLayout);
}

DXGI_FORMAT Rendering::InputLayout::ToDxFormat(ElementType type)
{
	switch (type)
	{
	case ElementType::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
	case ElementType::Float2: return DXGI_FORMAT_R32G32_FLOAT;
	case ElementType::Float: return DXGI_FORMAT_R32_FLOAT;
	default:
		Logger::PrintError("ElementType not supported");
		return DXGI_FORMAT_UNKNOWN;
	}
}

D3D11_INPUT_CLASSIFICATION Rendering::InputLayout::ToDx(SlotClass slotClass)
{
	switch (slotClass)
	{
	default: Logger::PrintError("Unknown SlotClass");
	case SlotClass::PerVertex: return D3D11_INPUT_PER_VERTEX_DATA;
	case SlotClass::PerInstance: return D3D11_INPUT_PER_INSTANCE_DATA;
	}
}

std::string Rendering::InputLayout::CreateDummyShaderString(const Element* pElements, int nrElements)
{
	std::stringstream ss;
	ss << "struct Vertex{\n";
	for (int i = 0; i < nrElements; i++)
	{
		const Element& element = pElements[i];
		ss << ToTypeString(element.Type) << " var" << i << ": " << element.Semantic << ";\n";
	}
	ss << "};\n";
	ss << "struct Pixel{float4 pos : VS_POSITION;};\n";
	ss << "Pixel vs_main(Vertex v){return (Pixel)0;}";
	return ss.str();
}

std::string Rendering::InputLayout::ToTypeString(ElementType type)
{
	switch (type)
	{
	case ElementType::Float3: return "float3";
	case ElementType::Float2: return "float2";
	case ElementType::Float: return "float";
	default:
		Logger::PrintError("ElementType not supported");
		return "";
	}
}
