#include "FbxAnimationLayer.h"

#include "Wrapping/AnimationLayer.h"

MyEngine::Io::Fbx::FbxAnimationLayer::FbxAnimationLayer(const Wrapping::AnimationLayer& animationLayer)
	: m_Name{ animationLayer.Name }
	, m_Id{ animationLayer.Id }
{
}
