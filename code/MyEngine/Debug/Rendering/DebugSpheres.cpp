#include "pch.h"
#include "DebugSpheres.h"

#include <Framework/Resources.h>
#include <Generation/Shapes.h>
#include <Rendering/State/InputLayout.h>
#include <Rendering/State/Mesh.h>

using namespace Rendering;

const InputLayout::Element DebugSpheres::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3},
	{"INST_POSITION", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance},
	{"INST_COLOR", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance},
	{"INST_SIZE", InputLayout::ElementType::Float, 1, InputLayout::SlotClass::PerInstance}
};

DebugSpheres::DebugSpheres(Gpu& gpu)
	: m_InputLayout{ gpu, ELEMENTS, static_cast<int>(sizeof(ELEMENTS) / sizeof(InputLayout::Element)) }
	, m_Shader{ gpu, Framework::Resources::GetGlobalShaderPath(L"LambertCam_Inst_Col_Pos.hlsl") }
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
	m_DrawBatch = IndexedDrawBatch{ gpu,
		vertices.GetData(), vertices.GetSize(),
		m_Spheres.GetData(), m_Spheres.GetCapacity(),
		indices.GetData(), indices.GetSize(),
		true, false, true };
}

void DebugSpheres::Render(const Gpu& gpu, const Float3& cameraPosition, const Float4X4& viewProjection)
{
	if (m_Spheres.GetSize() == 0) return;
	if (m_Spheres.GetSize() > m_DrawBatch.GetInstancesSize())
		m_DrawBatch.RecreateInstancesWithCapacity(gpu, m_Spheres, false);
	else
		m_DrawBatch.UpdateInstancesData(gpu, m_Spheres);
	m_DrawBatch.SetInstancesDrawCount(m_Spheres.GetSize());
	m_Spheres.Clear();

	m_InputLayout.Activate(gpu);
	m_Shader.Activate();
	m_DrawBatch.Draw(gpu);
}

void DebugSpheres::DrawSphere(const Float3& position, const Float3& color, float size)
{
	m_Spheres.Add({ position, color, size });
}
