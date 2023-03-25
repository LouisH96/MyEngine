#include "pch.h"
#include "BasicRenderer.h"

#include "App/Wrappers/Dx/InputLayout.h"

using namespace App;
using namespace Wrappers;
using namespace Wrappers::Dx;
using namespace MyEngine::Dx;


const InputLayout::Element Rendering::V_PosCol::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"COLOR", InputLayout::ElementType::Float3},
};

const InputLayout::Element Rendering::V_PosColNorm::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"COLOR", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3}
};
