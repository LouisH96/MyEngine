#pragma once
#include <cstdint>
#include <string>

#include "DataStructures/Array.h"
#include "DataStructures/List.h"
#include "Io/Fbx/Reading/Properties70.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxElement;
			}

			namespace Wrapping
			{
				class NodeAttribute;
				class AnimationLayer;
				class Model;
				class AnimationCurve;

				class AnimationCurveNode
				{
				public:
					//---| Types |---
					enum class NodeType
					{
						LockInfluenceWeights,
						Translation, Rotation, Scale,
						Visibility, FilmboxTypeId, FocalLength,
						Other
					};

					//---| Fields |---
					int64_t Id{};

					//---| Constructor/Destructor |---
					AnimationCurveNode() = default;
					explicit AnimationCurveNode(Reading::FbxElement& object);

					//---| Functions |---
					void AddNodeAttribute(const NodeAttribute& nodeAttribute);
					void AddAnimationCurve(const AnimationCurve& animationCurve);
					void SetParentModel(const Model& model);
					void SetAnimationLayer(const AnimationLayer& animationLayer);
					static NodeType GetNodeType(const std::string& typeName);

					void Print() const;
					NodeType GetNodeType() const { return m_NodeType; }
					bool IsTransformNode() const;
					const Model& GetModel() const { return *m_pParentModel; }
					const Array<const AnimationCurve*>& GetAnimationCurves() const { return m_AnimationCurves; }

					const Double3& GetDefaultVector() const { return m_Value.vectorValue; }
					bool GetDefaultBool() const { return m_Value.boolValue; }
					short GetDefaultShort() const { return m_Value.shortValue; }

					const AnimationLayer& GetAnimationLayer() const { return *m_pAnimationLayer; }

				private:
					List<const NodeAttribute*> m_NodeAttributes{};
					Array<const AnimationCurve*> m_AnimationCurves{};
					const Model* m_pParentModel{};
					const AnimationLayer* m_pAnimationLayer{};
					NodeType m_NodeType{};
					union
					{
						Double3 vectorValue;
						bool boolValue{};
						short shortValue;
					} m_Value{};
				};
			}
		}
	}
}
