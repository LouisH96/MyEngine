#include "pch.h"
#include "FbxAnimationLayer.h"

#include "Wrapping/AnimationLayer.h"

Io::Fbx::FbxAnimationLayer::FbxAnimationLayer(const Wrapping::AnimationLayer& animationLayer)
	: m_Name{ animationLayer.Name }
	, m_Id{ animationLayer.Id }
{
}
