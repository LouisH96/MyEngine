#pragma once
#include "FbxAnimationLayer.h"
#include "DataStructures/Array.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Wrapping
			{
				class AnimationStack;
			}

			class FbxAnimation
			{
			public:
				FbxAnimation() = default;
				explicit FbxAnimation(const Wrapping::AnimationStack& animationStack);

				const Array<FbxAnimationLayer>& GetLayers() const { return m_Layers; }
				int GetNrLayers() const { return m_Layers.GetSize(); }

				const int64_t& GetLocalStart() const { return m_LocalStart; }
				const int64_t& GetLocalStop() const { return m_LocalStop; }
				const int64_t& GetReferenceStart() const { return m_ReferenceStart; }
				const int64_t& GetReferenceStop() const { return m_ReferenceStop; }

			private:
				std::string m_Name;
				int64_t m_Id;
				int64_t m_LocalStart;
				int64_t m_LocalStop;
				int64_t m_ReferenceStart;
				int64_t m_ReferenceStop;

				Array<FbxAnimationLayer> m_Layers;
			};
		}
	}
}
