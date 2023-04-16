#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class Properties70;
				class FbxObject;
			}

			namespace Wrapping
			{
				class Model
				{
				public:
					Model() = default;
					Model(Reading::FbxObject& modelObject);

					const Math::Float3& GetRotationOffset() const { return m_RotationOffset; }
					const Math::Float3& GetRotationPivot() const { return m_RotationPivot; }

				private:
					int64_t m_Id{};
					std::string m_Name{};
					std::string m_TypeName{};
					int m_Version{};
					Math::Float3 m_RotationOffset{};
					Math::Float3 m_RotationPivot{};
					Math::Float3 m_ScalingPivot{};
					bool m_RotationActive{};
					int m_InheritType{};
					Math::Float3 m_ScalingMax{};
					int m_DefaultAttributeIndex{};
					Math::Float3 m_LclRotation{};
					std::string m_CurrentUvSet{};
					int m_Shading{};
					std::string m_Culling{};
				};
			}
		}
	}
}
