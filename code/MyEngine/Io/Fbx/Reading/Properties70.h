#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxProperty;
				class FbxObject;

				class Properties70
				{
				public:
					explicit Properties70(const FbxObject& object);

					Float3 GetFloat3(const std::string& name) const;
					Float3 GetFloat3(const std::string& name, const Float3& fallback) const;
					Double3 GetDouble3(const std::string& name) const;
					Double3 GetDouble3(const std::string& name, const Double3& fallback) const;
					Double3 GetDouble3(int idx) const;
					bool GetBool(const std::string& name) const;
					bool GetBool(const std::string& name, bool fallback) const;
					bool GetBool(int idx) const;
					int GetInt(const std::string& name) const;
					int GetInt(const std::string& name, int fallback) const;
					int64_t GetInt64(const std::string& name) const;
					int64_t GetInt64(const std::string& name, int64_t fallback) const;
					std::string GetString(const std::string& name) const;
					std::string GetString(const std::string& name, const std::string& fallback) const;
					double GetDouble(int idx) const;
					short GetShort(int idx) const;

					const FbxObject* GetProperty(const std::string& name) const;
					const FbxObject* GetPropertyOptional(const std::string& name) const;

					void Print(bool compact = true, int nrTabs = 0) const;

				private:
					const FbxObject& m_Object;
				};
			}
		}
	}
}
