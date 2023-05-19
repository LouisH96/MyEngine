#include "pch.h"
#include "FbxJoint.h"

#include "Debug/DebugRenderer.h"
#include "Io/Fbx/Wrapping/FbxData.h"

Io::Fbx::FbxJoint::FbxJoint(const Io::Fbx::Wrapping::Model& model, const Io::Fbx::Wrapping::FbxData& fbxData)
	: m_Name{ model.GetName() }
	, m_Curve(model)
{
	//POSITION
	const Float3 translation{ model.GetLclTranslation() };
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

	//
	m_LocalTranslation = translation;
	m_PreRotation = preRotation;
	m_PostRotation = { postRotationZ };
	m_PostRotation.RotateBy(postRotationY);
	m_PostRotation.RotateBy(postRotationX);
}

Io::Fbx::FbxJoint::FbxJoint(FbxJoint&& other) noexcept
	: m_Name{ std::move(other.m_Name) }
	, m_LocalTransform(std::move(other.m_LocalTransform))
	, m_Children{ std::move(other.m_Children) }
	, m_pParent{ std::move(other.m_pParent) }
	, m_Curve{ std::move(other.m_Curve) }
	, m_LocalTranslation{ std::move(other.m_LocalTranslation) }
	, m_PreRotation{ std::move(other.m_PreRotation) }
	, m_PostRotation{ std::move(other.m_PostRotation) }
{
	for (int i = 0; i < m_Children.GetSize(); i++)
		m_Children[i].m_pParent = this;
}

Io::Fbx::FbxJoint& Io::Fbx::FbxJoint::operator=(FbxJoint&& other) noexcept
{
	m_Name = std::move(other.m_Name);
	m_LocalTransform = std::move(other.m_LocalTransform);
	m_Children = std::move(other.m_Children);
	m_pParent = std::move(other.m_pParent);
	m_Curve = std::move(other.m_Curve);
	m_LocalTranslation = std::move(other.m_LocalTranslation);
	m_PreRotation = std::move(other.m_PreRotation);
	m_PostRotation = std::move(other.m_PostRotation);
	for (int i = 0; i < m_Children.GetSize(); i++)
		m_Children[i].m_pParent = this;
	return *this;
}

void Io::Fbx::FbxJoint::AddToDebugRender(float sphereSize) const
{
	AddToDebugRender({}, sphereSize);
}

void Io::Fbx::FbxJoint::AddToDebugRender(const Transform& parent, float sphereSize) const
{
	const Transform world{ Transform::LocalToWorld(m_LocalTransform, parent) };

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

void Io::Fbx::FbxJoint::AddToDebugRender(const int64_t& time, float sphereSize) const
{
	AddToDebugRender(time, {}, sphereSize);
}

void Io::Fbx::FbxJoint::AddToDebugRender(const int64_t& time, const Transform& parent, float sphereSize) const
{
	const Transform current{ m_Curve.AtTime(time) };

	Transform world{ parent };
	world = Transform::LocalToWorld({ current.Position, {} }, world);
	world = Transform::LocalToWorld({ {}, m_PreRotation }, world);
	world = Transform::LocalToWorld({ {}, current.Rotation }, world);
	world = Transform::LocalToWorld({ {}, m_PostRotation }, world);

	const Float3 position{ world.Position * .01f };
	if (m_pParent)
		DebugRenderer::AddLine(position, parent.Position * .01f, { 0,0,1 });

	DebugRenderer::AddSphere(position, { 0,0,1 }, sphereSize);
	for (int i = 0; i < m_Children.GetSize(); i++)
		m_Children[i].AddToDebugRender(time, world, sphereSize);
}
