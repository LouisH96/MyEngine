#include "pch.h"
#include "Joint.h"

#include "Debug/DebugRenderer.h"
#include "Io/Fbx/Wrapping/FbxData.h"

Animation::Joint::Joint(const Io::Fbx::Wrapping::Model& model, const Io::Fbx::Wrapping::FbxData& fbxData)
	: m_Name{ model.GetName() }
{
	//POSITION
	const Float3 translation{ model.GetLclTranslation() * 0.01f };
	const Quaternion preRotation{ Quaternion::FromEulerDegrees(model.GetPreRotation()) };

	const Float3& postEulers{ model.GetPostRotation() };
	const Quaternion postRotationX{ Quaternion::FromEulerDegrees({postEulers.x, 0, 0}) };
	const Quaternion postRotationY{ Quaternion::FromEulerDegrees({0, postEulers.y, 0}) };
	const Quaternion postRotationZ{ Quaternion::FromEulerDegrees({0, 0, postEulers.z}) };

	Quaternion rotation{ postRotationZ };
	rotation.RotateBy(postRotationY);
	rotation.RotateBy(postRotationX);
	rotation.RotateBy(preRotation);

	m_LocalTransform = { translation, rotation };

	//CHILDREN
	const Array<const Io::Fbx::Wrapping::Model*> children{ fbxData.GetChildren(model) };
	m_Children = { children.GetSize() };
	for (int i = 0; i < m_Children.GetSize(); i++)
	{
		m_Children[i] = { *children[i], fbxData };
		m_Children[i].m_pParent = this;
	}
}

Animation::Joint::Joint(Joint&& other) noexcept
	: m_Name{ std::move(other.m_Name) }
	, m_LocalTransform(std::move(other.m_LocalTransform))
	, m_Children{ std::move(other.m_Children) }
	, m_pParent{ std::move(other.m_pParent) }
{
	for (int i = 0; i < m_Children.GetSize(); i++)
		m_Children[i].m_pParent = this;
}

Animation::Joint& Animation::Joint::operator=(Joint&& other) noexcept
{
	m_Name = std::move(other.m_Name);
	m_LocalTransform = std::move(other.m_LocalTransform);
	m_Children = std::move(other.m_Children);
	m_pParent = std::move(other.m_pParent);
	for (int i = 0; i < m_Children.GetSize(); i++)
		m_Children[i].m_pParent = this;
	return *this;
}

void Animation::Joint::AddToDebugRender(float sphereSize) const
{
	AddToDebugRender({}, sphereSize);
}

void Animation::Joint::AddToDebugRender(const Transform& parent, float sphereSize) const
{
	const Game::Transform world{ Game::Transform::LocalToWorld(m_LocalTransform, parent) };

	if (m_pParent)
	{
		DebugRenderer::AddLine(world.Position, parent.Position, { 1,0,0 });
	}

	DebugRenderer::AddSphere(world.Position, { 1,0,0 }, sphereSize);
	for (int i = 0; i < m_Children.GetSize(); i++)
	{
		m_Children[i].AddToDebugRender(world, sphereSize);
	}
}
