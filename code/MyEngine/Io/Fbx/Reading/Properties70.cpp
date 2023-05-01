#include "pch.h"
#include "Properties70.h"

#include "FbxObject.h"
#include "Properties/FbxPropPrimitive.h"

Io::Fbx::Reading::Properties70::Properties70(const FbxObject& object)
	: m_Object{ object }
{
}

Math::Float3 Io::Fbx::Reading::Properties70::GetFloat3(const std::string& name) const
{
	const FbxObject& object{ *GetProperty(name) };
	return Math::Float3{
		static_cast<float>(object.GetProperty(object.GetNrProperties() - 3)->AsPrimitive<double>().GetValue()),
		static_cast<float>(object.GetProperty(object.GetNrProperties() - 2)->AsPrimitive<double>().GetValue()),
		static_cast<float>(object.GetProperty(object.GetNrProperties() - 1)->AsPrimitive<double>().GetValue())
	};
}

Math::Double3 Io::Fbx::Reading::Properties70::GetDouble3(const std::string& name) const
{
	const FbxObject& object{ *GetProperty(name) };
	return Math::Double3{
		object.GetProperty(object.GetNrProperties() - 3)->AsPrimitive<double>().GetValue(),
		object.GetProperty(object.GetNrProperties() - 2)->AsPrimitive<double>().GetValue(),
		object.GetProperty(object.GetNrProperties() - 1)->AsPrimitive<double>().GetValue()
	};
}

bool Io::Fbx::Reading::Properties70::GetBool(const std::string& name) const
{
	const FbxObject& object{ *GetProperty(name) };
	return object.GetProperty(object.GetNrProperties() - 1)->AsPrimitive<int>().GetValue() == 1;
}

int Io::Fbx::Reading::Properties70::GetInt(const std::string& name) const
{
	const FbxObject& object{ *GetProperty(name) };
	return object.GetProperty(object.GetNrProperties() - 1)->AsPrimitive<int>().GetValue();
}

std::string Io::Fbx::Reading::Properties70::GetString(const std::string& name) const
{
	const FbxObject& object{ *GetProperty(name) };
	return object.GetProperty(object.GetNrProperties() - 1)->AsString();
}

Math::Float3 Io::Fbx::Reading::Properties70::GetFloat3(const std::string& name, const Math::Float3& fallback) const
{
	const FbxObject* pObject{ GetPropertyOptional(name) };
	if (pObject == nullptr) return fallback;
	return Math::Float3{
		static_cast<float>(pObject->GetProperty(pObject->GetNrProperties() - 3)->AsPrimitive<double>().GetValue()),
		static_cast<float>(pObject->GetProperty(pObject->GetNrProperties() - 2)->AsPrimitive<double>().GetValue()),
		static_cast<float>(pObject->GetProperty(pObject->GetNrProperties() - 1)->AsPrimitive<double>().GetValue())
	};
}

Math::Double3 Io::Fbx::Reading::Properties70::GetDouble3(const std::string& name, const Math::Double3& fallback) const
{
	const FbxObject* pObject{ GetPropertyOptional(name) };
	if (pObject == nullptr) return fallback;
	return Math::Double3{
		pObject->GetProperty(pObject->GetNrProperties() - 3)->AsPrimitive<double>().GetValue(),
		pObject->GetProperty(pObject->GetNrProperties() - 2)->AsPrimitive<double>().GetValue(),
		pObject->GetProperty(pObject->GetNrProperties() - 1)->AsPrimitive<double>().GetValue()
	};
}

bool Io::Fbx::Reading::Properties70::GetBool(const std::string& name, bool fallback) const
{
	const FbxObject* pObject{ GetPropertyOptional(name) };
	if (pObject == nullptr) return fallback;
	return pObject->GetProperty(pObject->GetNrProperties() - 1)->AsPrimitive<int>().GetValue() == 1;
}

int Io::Fbx::Reading::Properties70::GetInt(const std::string& name, int fallback) const
{
	const FbxObject* pObject{ GetPropertyOptional(name) };
	if (pObject == nullptr) return fallback;
	return pObject->GetProperty(pObject->GetNrProperties() - 1)->AsPrimitive<int>().GetValue();
}

std::string Io::Fbx::Reading::Properties70::GetString(const std::string& name, const std::string& fallback) const
{
	const FbxObject* pObject{ GetPropertyOptional(name) };
	if (pObject == nullptr) return fallback;
	return pObject->GetProperty(pObject->GetNrProperties() - 1)->AsString();
}

const Io::Fbx::Reading::FbxObject* Io::Fbx::Reading::Properties70::GetPropertyOptional(const std::string& name) const
{
	for (int i = 0; i < m_Object.GetChildren().size(); i++)
		if (m_Object.GetChildren()[i]->GetProperties().size() > 0 //check is needed only for version 7700
			&& m_Object.GetChildren()[i]->GetProperty(0)->AsString() == name)
			return m_Object.GetChildren()[i];
	return nullptr;
}

const Io::Fbx::Reading::FbxObject* Io::Fbx::Reading::Properties70::GetProperty(const std::string& name) const
{
	for (int i = 0; i < m_Object.GetChildren().size(); i++)
		if (m_Object.GetChildren()[i]->GetProperty(0)->AsString() == name)
			return m_Object.GetChildren()[i];
	Logger::PrintError("Property(" + name + ") not found");
	return nullptr;
}
