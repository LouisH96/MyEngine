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
				//---| Construction |---
				FbxAnimation() = default;
				explicit FbxAnimation(const Wrapping::AnimationStack& animationStack);
				~FbxAnimation() = default;

				FbxAnimation(const FbxAnimation& other) = delete;
				FbxAnimation& operator= (const FbxAnimation& other) = delete;
				FbxAnimation(FbxAnimation&& other) = default;
				FbxAnimation& operator= (FbxAnimation&& other) = default;

				//---| Functions |---
				const Array<FbxAnimationLayer>& GetLayers() const { return m_Layers; }
				int GetNrLayers() const { return m_Layers.GetSize(); }

				const int64_t& GetId() const { return m_Id; }
				const uint64_t& GetLocalStart() const { return m_LocalStart; }
				const uint64_t& GetLocalStop() const { return m_LocalStop; }
				const uint64_t& GetReferenceStart() const { return m_ReferenceStart; }
				const uint64_t& GetReferenceStop() const { return m_ReferenceStop; }

				float GetDuration() const { return GetLocalDuration(); }
				float GetLocalDuration() const;
				float GetReferenceDuration() const;

			private:
				std::string m_Name;
				int64_t m_Id{};
				uint64_t m_LocalStart{};
				uint64_t m_LocalStop{};
				uint64_t m_ReferenceStart{};
				uint64_t m_ReferenceStop{};

				Array<FbxAnimationLayer> m_Layers;
			};
		}
	}
}
