#include "pch.h"
#include "FbxJoint.h"

#include "FbxClass.h"
#include "Debug/Rendering/DebugRenderer.h"
#include "Io/Fbx/Wrapping/FbxData.h"

Io::Fbx::FbxJoint::FbxJoint(
	const Wrapping::Model& model,
	const Wrapping::FbxData& fbxData,
	const FbxClass& fbxClass)
	: m_Name{ model.GetName() }
	, m_Curves{ fbxClass.GetNrOfAnimationLayers() }
{
	//ANIMATION
	for (int iAnimation = 0, iCurve = 0; iAnimation < fbxClass.GetAnimations().GetSize(); iAnimation++)
	{
		const FbxAnimation& animation{ fbxClass.GetAnimations()[iAnimation] };
		for (int iLayer = 0; iLayer < animation.GetLayers().GetSize(); iLayer++, iCurve++)
			m_Curves[iCurve] = FbxTransformCurve{ model, animation.GetLayers()[iLayer] };
	}

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
	const Array<const Wrapping::Model*> children{ fbxData.GetChildren(model) };
	m_Children = { children.GetSize() };
	for (int i = 0; i < m_Children.GetSize(); i++)
	{
		m_Children[i] = { *children[i], fbxData, fbxClass };
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
	, m_Curves{ std::move(other.m_Curves) }
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
	m_Curves = std::move(other.m_Curves);
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

void Io::Fbx::FbxJoint::AddToDebugRender(const FbxAnimationLayer& layer, const int64_t& time, float scale, float sphereSize) const
{
	AddToDebugRender(layer, time, {}, scale, sphereSize);
}

void Io::Fbx::FbxJoint::AddToDebugRender(const FbxAnimationLayer& layer, const int64_t& time, const Transform& parent, float scale, float sphereSize) const
{
	const FbxTransformCurve* pCurve{ FindCurve(layer) };
	if (!pCurve)
		Logger::PrintError("Curve for layer not found");
	const Transform current{ pCurve->AtTime(time) };

	Transform world{ parent };
	world = Transform::LocalToWorld({ current.Position, {} }, world);
	world = Transform::LocalToWorld({ {}, m_PreRotation }, world);
	world = Transform::LocalToWorld({ {}, current.Rotation }, world);
	world = Transform::LocalToWorld({ {}, m_PostRotation }, world);

	const Float3 position{ world.Position * scale };
	if (m_pParent)
		DebugRenderer::DrawLine(position, parent.Position * scale, { 0,0,1 });

	DebugRenderer::DrawSphere(position, { 0,0,1 }, sphereSize);
	for (int i = 0; i < m_Children.GetSize(); i++)
		m_Children[i].AddToDebugRender(layer, time, world, scale, sphereSize);
}

const Io::Fbx::FbxTransformCurve* Io::Fbx::FbxJoint::FindCurve(const FbxAnimationLayer& layer) const
{
	for (int i = 0; i < m_Curves.GetSize(); i++)
		if (m_Curves[i].IsInLayer(layer))
			return &m_Curves[i];
	return nullptr;
}
