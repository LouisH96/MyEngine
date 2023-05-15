#include "pch.h"
#include "FbxJoint.h"

#include "Debug/DebugRenderer.h"
#include "Game/Transform.h"
#include "Math/Quaternion.h"
#include "Wrapping/FbxData.h"
#include "Wrapping/Model.h"

Io::Fbx::FbxJoint::FbxJoint(const Wrapping::Model& model, const Wrapping::FbxData& fbxData)
	: FbxJoint{ model, fbxData, {} }
{
}

Io::Fbx::FbxJoint::FbxJoint(const Wrapping::Model& model, const Wrapping::FbxData& fbxData, const Game::Transform& parentTransform)
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

	const Game::Transform transform{ Game::Transform::LocalToWorld({translation, rotation}, parentTransform) };
	m_WorldPosition = transform.Position;

	//CHILDREN
	const Array<const Wrapping::Model*> children{ fbxData.GetChildren(model) };
	m_Children = { children.GetSize() };
	for (int i = 0; i < m_Children.GetSize(); i++)
	{
		m_Children[i] = { *children[i], fbxData, transform };
		m_Children[i].m_pParent = this;
	}
}

Io::Fbx::FbxJoint::FbxJoint(FbxJoint&& other) noexcept
	: m_Name{ std::move(other.m_Name) }
	, m_WorldPosition{ std::move(other.m_WorldPosition) }
	, m_Children{ std::move(other.m_Children) }
	, m_pParent{ std::move(other.m_pParent) }
{
	for (int i = 0; i < m_Children.GetSize(); i++)
		m_Children[i].m_pParent = this;
}

Io::Fbx::FbxJoint& Io::Fbx::FbxJoint::operator=(FbxJoint&& other) noexcept
{
	m_Name = std::move(other.m_Name);
	m_WorldPosition = std::move(other.m_WorldPosition);
	m_Children = std::move(other.m_Children);
	m_pParent = std::move(other.m_pParent);
	for (int i = 0; i < m_Children.GetSize(); i++)
		m_Children[i].m_pParent = this;
	return *this;
}

void Io::Fbx::FbxJoint::AddToDebugRender(float sphereSize) const
{
	if(m_pParent)
	{
		DebugRenderer::AddLine(m_WorldPosition, m_pParent->m_WorldPosition, { 1,0,0 });
	}

	DebugRenderer::AddSphere(m_WorldPosition, { 1,0,0 }, sphereSize);
	for (int i = 0; i < m_Children.GetSize(); i++)
	{
		m_Children[i].AddToDebugRender(sphereSize);
	}
}
