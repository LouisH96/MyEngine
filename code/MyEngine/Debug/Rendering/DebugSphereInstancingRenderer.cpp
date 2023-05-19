#include "pch.h"
#include "DebugSphereInstancingRenderer.h"

#include "Framework/Resources.h"
#include "Generation/Shapes.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/InputLayout.h"
#include "Rendering/State/Mesh.h"

using namespace Rendering;

const InputLayout::Element DebugSphereInstancingRenderer::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3},
	{"INST_POSITION", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance},
	{"INST_COLOR", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance},
	{"INST_SIZE", InputLayout::ElementType::Float, 1, InputLayout::SlotClass::PerInstance}
};

DebugSphereInstancingRenderer::DebugSphereInstancingRenderer(Gpu& gpu)
	: m_BlendState{ gpu }
	, m_RasterizerState{ gpu }
	, m_InputLayout{ gpu, ELEMENTS, static_cast<int>(sizeof(ELEMENTS) / sizeof(InputLayout::Element)) }
	, m_Shader{ gpu, Framework::Resources::GetGlobalShaderPath(L"LambertCam_Inst_Col_Pos.hlsl") }
	, m_ConstantBuffer{ gpu }
{
	using Vertex = V_PosNor;

	//VERTEX BUFFER
	const Sphere sphere{ {0,0,0}, 1 };
	Array<Float3> positions{};
	Array<Float3> normals{};
	Array<int> indices{};
	Generation::Shapes::GenerateSphereBuffers(sphere, 20, 10, positions, normals, indices);
	Array<Vertex> vertices{ positions.GetSize() };
	for (int i = 0; i < positions.GetSize(); i++)
		vertices[i] = Vertex{ positions[i], normals[i] };

	//INSTANCE BUFFER
	m_Spheres.Add({ { 1,0,1 }, { 1,0,0 }, .1f });
	m_Spheres.Add({ { 2,0,2 }, { 1,0,0 }, .1f });
	m_Spheres.Add({ { 3,0,3 }, { 1,0,0 }, .1f });

	m_DrawBatch = IndexedDrawBatch{ gpu,
		vertices.GetData(), vertices.GetSize(),
		m_Spheres.GetData(), m_Spheres.GetCapacity(),
		indices.GetData(), indices.GetSize(),
		true, false, true };
}

void DebugSphereInstancingRenderer::Render(const Gpu& gpu, const Float3& cameraPosition, const Float4X4& viewProjection)
{
	m_Spheres.Add({ { 1,0,1 }, { 1,0,0 }, .1f });
	m_Spheres.Add({ { 2,0,2 }, { 1,0,0 }, .1f });
	m_Spheres.Add({ { 3,0,3 }, { 1,0,0 }, .1f });
	m_Spheres.Add({ { 1,1,1 }, { 1,0,0 }, .1f });
	m_Spheres.Add({ { 2,1,2 }, { 1,0,0 }, .1f });
	m_Spheres.Add({ { 3,1,3 }, { 1,0,0 }, .1f });

	if (m_Spheres.GetSize() == 0) return;
	if (m_Spheres.GetSize() > m_DrawBatch.GetInstancesSize())
		m_DrawBatch.RecreateInstancesWithCapacity(gpu, m_Spheres, false);
	else
	{
		m_DrawBatch.UpdateInstancesData(gpu, m_Spheres);
		m_DrawBatch.SetInstancesDrawCount(m_Spheres.GetSize());
	}
	m_Spheres.Clear();

	m_ConstantBuffer.Update(gpu, CB_CamMatPos{ cameraPosition, viewProjection });
	m_ConstantBuffer.Activate(gpu);
	m_RasterizerState.Activate(gpu);
	m_InputLayout.Activate(gpu);
	m_BlendState.Activate(gpu);
	m_Shader.Activate();

	m_DrawBatch.Draw(gpu);
}

void DebugSphereInstancingRenderer::DrawSphere(const Float3& position, const Float3& color, float size)
{
	m_Spheres.Add({ position, color, size });
}
