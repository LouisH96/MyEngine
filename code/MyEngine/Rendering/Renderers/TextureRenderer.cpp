#include "pch.h"
#include "TextureRenderer.h"

const Dx::InputLayout::Element Rendering::V_PosNorUv::ELEMENTS[] =
{
	{"POSITION", Dx::InputLayout::ElementType::Float3},
	{"NORMAL", Dx::InputLayout::ElementType::Float3},
	{"TEXCOORD", Dx::InputLayout::ElementType::Float2}
};