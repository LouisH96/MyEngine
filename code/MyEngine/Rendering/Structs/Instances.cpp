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

const Rendering::InputLayout::Element Rendering::I_ModelMatrices::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3},
	{"TEXCOORD", InputLayout::ElementType::Float2},

	{"INST_MODEL", InputLayout::ElementType::Float4X4, 1, InputLayout::SlotClass::PerInstance},
	{"INST_MODEL_VIEW_PROJECTION", InputLayout::ElementType::Float4X4, 1, InputLayout::SlotClass::PerInstance}
};