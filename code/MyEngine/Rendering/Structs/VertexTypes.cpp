#include "pch.h"
#include "VertexTypes.h"

const Rendering::InputLayout::Element Rendering::V_Pos2::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float2}
};
const int Rendering::V_Pos2::NR_ELEMENTS = sizeof ELEMENTS / sizeof(InputLayout::Element);

const Rendering::InputLayout::Element Rendering::V_Pos2Col::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float2},
	{"COLOR", InputLayout::ElementType::Float3},
};

const Rendering::InputLayout::Element Rendering::V_Pos2Uv::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float2},
	{"TEXCOORD", InputLayout::ElementType::Float2},
};

const Rendering::InputLayout::Element Rendering::V_PosNor::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3},
};

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

const Rendering::InputLayout::Element Rendering::V_PosNorCol::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3},
	{"COLOR", InputLayout::ElementType::Float3}
};

const Rendering::InputLayout::Element Rendering::V_PosUv::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"TEXCOORD", InputLayout::ElementType::Float2}
};

const Rendering::InputLayout::Element Rendering::V_PosNorUv::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3},
	{"TEXCOORD", InputLayout::ElementType::Float2}
};

Array<Rendering::V_Pos2> Rendering::V_Pos2::GetCenterRect()
{
	Array<V_Pos2> vertices{4};
	vertices[0].pos = { -.5f,-.5f };
	vertices[1].pos = { -.5f,.5f };
	vertices[2].pos = { .5f,-.5f };
	vertices[3].pos = { .5f,.5f };
	return vertices;
}

Array<Rendering::V_Pos2Uv> Rendering::V_Pos2Uv::GetCenterRect()
{
	Array<V_Pos2Uv> vertices{4};
	vertices[0] = { { -.5f, -.5f }, {0,0} };
	vertices[1] = { { -.5f,.5f }, {0,-1} };
	vertices[2] = { { .5f,-.5f }, {1,0} };
	vertices[3] = { { .5f,.5f }, {1,-1} };
	return vertices;
}
