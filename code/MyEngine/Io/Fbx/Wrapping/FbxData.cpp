#include "pch.h"
#include "FbxData.h"

#include "../Reading/FbxObject.h"
#include "../Reading/FbxReader.h"

using namespace Math;

Io::Fbx::Wrapping::FbxData::FbxData(const std::wstring& fbxPath)
	: FbxData(Fbx::Reading::FbxReader{ fbxPath })
{}

Io::Fbx::Wrapping::FbxData::FbxData(Reading::FbxReader&& reader)
{
	const Reading::FbxObject& objects{ *reader.GetRoot().GetChild("Objects") };

	reader.GetRoot().Print();

	//Geometry
	const std::vector<Reading::FbxObject*> geometries{ objects.GetChildren("Geometry") };
	m_Geometries = { static_cast<int>(geometries.size()) };
	for(int i = 0; i < geometries.size(); i++)
		m_Geometries[i] = Geometry{*geometries[i]};

	//Models
	const std::vector<Reading::FbxObject*> models{ objects.GetChildren("Model") };
	if (models.size() != geometries.size())
		Logger::PrintWarning("Expected same nr of models & geometries");
	m_Models= { static_cast<int>(models.size()) };
	for (int i = 0; i < m_Models.GetSize(); i++)
		m_Models[i] = Model{ *models[i] };
}
