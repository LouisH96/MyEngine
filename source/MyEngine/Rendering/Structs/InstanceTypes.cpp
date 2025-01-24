#include "pch.h"
#include "InstanceTypes.h"

using namespace Rendering;

const InputLayout::Element I_Pos2Col::ELEMENTS[] =
{
	{"INST_OFFSET", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_SIZE", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_COLOR", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance }
};

bool I_Pos2Col::IsValid() const
{
	return size.x != 0;
}

void I_Pos2Col::Invalidate()
{
	size.x = 0;
}

const InputLayout::Element I_Pos2UvCol::ELEMENTS[] =
{
	{"INST_POS_OFFSET", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_SIZE", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_UV_SCALE", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_UV_OFFSET", InputLayout::ElementType::Float2, 1, InputLayout::SlotClass::PerInstance},
	{"INST_COLOR", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance}
};

bool I_Pos2UvCol::IsValid() const
{
	return posScale.x != 0;
}

void I_Pos2UvCol::Invalidate()
{
	posScale.x = 0;
}

const InputLayout::Element I_ModelMatrix::ELEMENTS[] =
{
	{"INST_MODEL", InputLayout::ElementType::Float4X4, 1, InputLayout::SlotClass::PerInstance},
};

const InputLayout::Element I_ModelMatrices::ELEMENTS[] =
{
	{"INST_MODEL", InputLayout::ElementType::Float4X4, 1, InputLayout::SlotClass::PerInstance},
	{"INST_MODEL_VIEW_PROJECTION", InputLayout::ElementType::Float4X4, 1, InputLayout::SlotClass::PerInstance}
};

const InputLayout::Element I_PosCol::ELEMENTS[] =
{
	{"INST_POS", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance},
	{"INST_COLOR", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance}
};

bool I_PosCol::IsValid() const
{
	return pos.y >= 8192.f;
}

void I_PosCol::Invalidate()
{
	pos.y = 8192.f;
}