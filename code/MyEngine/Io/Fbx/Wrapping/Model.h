#pragma once
#include "Connection.h"
#include "Pose.h"

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

					const int64_t& GetId() const { return m_Id; }
					const std::string& GetName() const { return m_Name; }
					const std::string& GetTypeName() const { return m_TypeName; }
					int GetVersion() const { return m_Version; }
					const Float3& GetPreRotation() const { return m_PreRotation; }
					const Float3& GetRotationOffset() const { return m_RotationOffset; }
					const Float3& GetRotationPivot() const { return m_RotationPivot; }
					const Float3& GetScalingPivot() const { return m_ScalingPivot; }
					bool GetRotationActive() const { return m_RotationActive; }
					int GetInheritType() const { return m_InheritType; }
					const Float3& GetScalingMax() const { return m_ScalingMax; }
					int GetDefaultAttributeIndex() const { return m_DefaultAttributeIndex; }
					const Float3& GetLclTranslation() const { return m_LclTranslation; }
					const Float3& GetLclRotation() const { return m_LclRotation; }
					const Float3& GetLclScaling() const { return m_LclScaling; }
					const std::string& GetCurrentUvSet() const { return m_CurrentUvSet; }
					int GetShading() const { return m_Shading;  }
					const std::string& GetCulling() const { return m_Culling; }

					std::string& GetName() { return m_Name; }

					void SetBindPose(const Pose::Node& pose);
					void AddConnection(const Connection& connection);

					const Pose::Node& GetBindPose() const { return m_BindPose; }
					const Array<Connection>& GetConnections() const { return m_Connections; }

				private:
					int64_t m_Id{};
					std::string m_Name{};
					std::string m_TypeName{};
					int m_Version{};
					Float3 m_PreRotation{};
					Float3 m_RotationOffset{};
					Float3 m_RotationPivot{};
					Float3 m_ScalingPivot{};
					bool m_RotationActive{};
					int m_InheritType{};
					Float3 m_ScalingMax{};
					int m_DefaultAttributeIndex{};
					Float3 m_LclTranslation{};
					Float3 m_LclRotation{};
					Float3 m_LclScaling{};
					std::string m_CurrentUvSet{};
					int m_Shading{};
					std::string m_Culling{};

					Array<Connection> m_Connections{};
					Pose::Node m_BindPose{};
				};
			}
		}
	}
}
