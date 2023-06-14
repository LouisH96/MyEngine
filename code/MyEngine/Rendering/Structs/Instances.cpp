#include "pch.h"
#include "Instances.h"

const Rendering::InputLayout::Element Rendering::I_Pos2UvCol::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float2},
	{"TEXCOORD", InputLayout::ElementType::Float2},
	{"INST_POS_OFFSET", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_SIZE", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_UV_SCALE", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_UV_OFFSET", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_COLOR", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance}
};