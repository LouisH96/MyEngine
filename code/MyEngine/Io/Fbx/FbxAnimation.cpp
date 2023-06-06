#include "pch.h"
#include "FbxAnimation.h"

#include "Wrapping/AnimationStack.h"

Io::Fbx::FbxAnimation::FbxAnimation(const Wrapping::AnimationStack& animationStack)
	: m_Name{ animationStack.Name }
	, m_Id{ animationStack.Id }
	, m_LocalStart{ animationStack.LocalStart }
	, m_LocalStop{ animationStack.LocalStop }
	, m_ReferenceStart{ animationStack.ReferenceStart }
	, m_ReferenceStop{ animationStack.ReferenceStop }
	, m_Layers{ animationStack.GetAnimationLayers().GetSize() }
{
	for (int i = 0; i < animationStack.GetAnimationLayers().GetSize(); i++)
		m_Layers[i] = FbxAnimationLayer{ *animationStack.GetAnimationLayers()[i] };
}
