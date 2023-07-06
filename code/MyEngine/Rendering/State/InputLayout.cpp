#include "pch.h"
#include "InputLayout.h"

#include <d3dcompiler.h>

#include <sstream>
#include <Rendering/Dx/DxHelper.h>

Rendering::InputLayout::InputLayout(const Element* pElements, int nrElements)
{
	//CREATE INPUT_ELEMENT_DESC
	const Array<D3D11_INPUT_ELEMENT_DESC> dxDescriptions{ToDxDescriptions(pElements, nrElements)};

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
		std::cout << "Shader: \n" << dummyShaderText << std::endl;
	}

	//INPUTLAYOUT
	result = Globals::pGpu->GetDevice().CreateInputLayout(
		dxDescriptions.GetData(), dxDescriptions.GetSize(),
		pBlob->GetBufferPointer(), pBlob->GetBufferSize(),
		&m_pInputLayout);
	if (FAILED(result))
		Logger::PrintError("CreateInputLayout");

	SAFE_RELEASE(pBlob);
	SAFE_RELEASE(pErrorBlob);
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
	case ElementType::Float4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case ElementType::Float3: return DXGI_FORMAT_R32G32B32_FLOAT;
	case ElementType::Float2: return DXGI_FORMAT_R32G32_FLOAT;
	case ElementType::Float: return DXGI_FORMAT_R32_FLOAT;
	case ElementType::UInt: return DXGI_FORMAT_R32_UINT;
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
	int iVar = 0;
	for (int i = 0; i < nrElements; i++)
	{
		const Element& element = pElements[i];
		switch (element.Type)
		{
		case ElementType::Float4X4:
			for (int iRow = 0; iRow < 4; iRow++)
				ss << ToTypeString(ElementType::Float4) << " var" << iVar++ << ": " << element.Semantic << iRow << ";\n";
			break;
		default:
			ss << ToTypeString(element.Type) << " var" << iVar++ << ": " << element.Semantic << ";\n";
			break;
		}
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
	case ElementType::Float4: return "float4";
	case ElementType::Float3: return "float3";
	case ElementType::Float2: return "float2";
	case ElementType::Float: return "float";
	case ElementType::UInt: return "uint";
	default:
		Logger::PrintError("ElementType not supported");
		return "";
	}
}

unsigned Rendering::InputLayout::ToNrDxElements(ElementType type)
{
	switch (type)
	{
	case ElementType::Float4X4: return 4;
	default: return 1;
	}
}

void Rendering::InputLayout::AddDxElements(D3D11_INPUT_ELEMENT_DESC*& dxElements, const Element& element)
{
	DXGI_FORMAT dxFormat;
	switch (element.Type)
	{
	case ElementType::Float4X4:
		dxFormat = ToDxFormat(ElementType::Float4);
		break;
	default:
		dxFormat = ToDxFormat(element.Type);
		break;
	}

	for (int i = 0; i < ToNrDxElements(element.Type); i++)
	{
		dxElements->SemanticName = element.Semantic.c_str();
		dxElements->SemanticIndex = i;
		dxElements->Format = dxFormat;
		dxElements->InputSlot = element.InputSlot;
		dxElements->AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		dxElements->InputSlotClass = ToDx(element.SlotClass);
		dxElements->InstanceDataStepRate = element.SlotClass == SlotClass::PerInstance ? 1 : 0;
		dxElements++;
	}
}

Array<D3D11_INPUT_ELEMENT_DESC> Rendering::InputLayout::ToDxDescriptions(const Element* pElements, int nrElements)
{
	unsigned nrDxElements{ 0 };
	for (int i = 0; i < nrElements; i++)
		nrDxElements += ToNrDxElements(pElements[i].Type);

	Array<D3D11_INPUT_ELEMENT_DESC> dxElements{nrDxElements};
	D3D11_INPUT_ELEMENT_DESC* pDesc{ dxElements.GetData() };
	for (int i = 0; i < nrElements; i++)
		AddDxElements(pDesc, pElements[i]);

	return dxElements;
}
