#include "pch.h"
#include "VertexTypes.h"

const Rendering::InputLayout::Element Rendering::V_PosCol::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"COLOR", InputLayout::ElementType::Float3},
};

const Rendering::InputLayout::Element Rendering::V_PosColNorm::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"COLOR", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3}
};

const Rendering::InputLayout::Element Rendering::V_PosNorUv::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3},
	{"TEXCOORD", InputLayout::ElementType::Float2}
};