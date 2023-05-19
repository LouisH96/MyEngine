#include "pch.h"
#include "DebugSphereInstancingRenderer.h"

#include "Framework/Resources.h"
#include "Generation/Shapes.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/Mesh.h"

using namespace Rendering;

const InputLayout::Element DebugSphereInstancingRenderer::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"COLOR", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3},
	{"INSTANCEPOS", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance}
};

DebugSphereInstancingRenderer::DebugSphereInstancingRenderer(Gpu& gpu)
	: m_BlendState{ gpu }
	, m_RasterizerState{ gpu }
	, m_InputLayout{ gpu, ELEMENTS, static_cast<int>(sizeof(ELEMENTS) / sizeof(InputLayout::Element)) }
	, m_Shader{ gpu, Framework::Resources::GetGlobalShaderPath(L"LambertCam_Inst_Col_Pos.hlsl") }
	, m_ConstantBuffer{ gpu }
{
	using Vertex = V_PosColNorm;

	//VERTEX BUFFER
	const Sphere sphere{ {0,0,0}, .5f };
	Array<Float3> positions{};
	Array<Float3> normals{};
	Array<int> indices{};
	Generation::Shapes::GenerateSphereBuffers(sphere, 20, 10, positions, normals, indices);
	Array<Vertex> vertices{ positions.GetSize() };
	for (int i = 0; i < positions.GetSize(); i++)
		vertices[i] = Vertex{ positions[i], {1,1,0}, normals[i] };

	//INSTANCE BUFFER
	Array<Float3> instancePosition{3};
	instancePosition[0] = {};
	instancePosition[1] = { 1,0,1 };
	instancePosition[2] = { 1,1,1 };

	m_DrawBatch = IndexedDrawBatch{ gpu, vertices, instancePosition, indices };
}

void DebugSphereInstancingRenderer::Render(const Gpu& gpu, const Float3& cameraPosition, const Float4X4& viewProjection)
{
	m_ConstantBuffer.Update(gpu, CB_CamMatPos{cameraPosition, viewProjection});
	m_ConstantBuffer.Activate(gpu);
	m_RasterizerState.Activate(gpu);
	m_InputLayout.Activate(gpu);
	m_BlendState.Activate(gpu);
	m_Shader.Activate();

	m_DrawBatch.Draw(gpu);
}
