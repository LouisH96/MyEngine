#include "pch.h"
#include "DebugSpheres.h"

#include <Framework/Resources.h>
#include <Generation/Shapes.h>
#include <Rendering/DrawData/InstanceIdxList.h>
#include <Rendering/State/InputLayout.h>

#include "Geometry/Shapes/Sphere.h"

using namespace Rendering;

const InputLayout::Element DebugSpheres::ELEMENTS[] =
{
	{"POSITION", InputLayout::ElementType::Float3},
	{"NORMAL", InputLayout::ElementType::Float3},
	{"INST_POSITION", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance},
	{"INST_COLOR", InputLayout::ElementType::Float3, 1, InputLayout::SlotClass::PerInstance},
	{"INST_SIZE", InputLayout::ElementType::Float, 1, InputLayout::SlotClass::PerInstance}
};

DebugSpheres::DebugSpheres()
	: m_InputLayout{ ELEMENTS, static_cast<int>(sizeof(ELEMENTS) / sizeof(InputLayout::Element)) }
	, m_Shader{ Resources::GlobalShader(L"Debug_Spheres.hlsl") }
{
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
	m_Spheres = InstanceIdxList<Vertex, Instance>{
		vertices.GetData(), vertices.GetSizeU(),
		indices.GetData(), indices.GetSizeU() };
}

void DebugSpheres::Render()
{
	if (m_Spheres.IsEmpty()) return;

	m_InputLayout.Activate();
	m_Shader.Activate();
	m_Spheres.Draw();
	m_Spheres.Clear();
}

void DebugSpheres::DrawSphere(const Float3& position, const Float3& color, float size)
{
	m_Spheres.Add({ position, color, size });
}

void DebugSpheres::DrawSphere(const Sphere& sphere, const Float3& color)
{
	m_Spheres.Add({ sphere.GetCenter(), color, sphere.GetRadius() });
}
