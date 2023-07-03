#include "pch.h"
#include "InstanceTypes.h"

const Rendering::InputLayout::Element Rendering::I_Pos2Col::ELEMENTS[] =
{
	{"INST_OFFSET", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_SIZE", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_COLOR", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance }
};
const int Rendering::I_Pos2Col::NR_ELEMENTS = sizeof ELEMENTS / sizeof(InputLayout::Element);

bool Rendering::I_Pos2Col::IsValid() const
{
	return size.x != 0;
}

void Rendering::I_Pos2Col::Invalidate()
{
	size = 0;
}

const Rendering::InputLayout::Element Rendering::I_Pos2UvCol::ELEMENTS[] =
{
	{"INST_POS_OFFSET", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_SIZE", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_UV_SCALE", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_UV_OFFSET", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_COLOR", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance}
};

bool Rendering::I_Pos2UvCol::IsValid() const
{
	return posScale.x != 0;
}

void Rendering::I_Pos2UvCol::Invalidate()
{
	posScale.x = 0;
}

const Rendering::InputLayout::Element Rendering::I_ModelMatrices::ELEMENTS[] =
{
	{"INST_MODEL", InputLayout::ElementType::Float4X4, 1, InputLayout::SlotClass::PerInstance},
	{"INST_MODEL_VIEW_PROJECTION", InputLayout::ElementType::Float4X4, 1, InputLayout::SlotClass::PerInstance}
};