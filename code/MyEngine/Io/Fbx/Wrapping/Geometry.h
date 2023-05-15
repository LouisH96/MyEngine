#pragma once
#include "Math/Float2.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject;
			}

			namespace Wrapping
			{
				class Geometry
				{
				public:
					Geometry() = default;
					explicit Geometry(Reading::FbxObject& geometryObject);

					Array<Math::Float3>& GetPoints() { return m_Points; }
					Array<Math::Float3>& GetNormals() { return m_Normals; }
					Array<Math::Float2>& GetUvs() { return m_Uvs; }
					Array<int>& GetIndices() { return m_Indices; }
					const Array<Math::Float3>& GetPoints() const { return m_Points; }
					const Array<Math::Float3>& GetNormals() const { return m_Normals; }
					const Array<Math::Float2>& GetUvs() const { return m_Uvs; }
					const Array<int>& GetIndices() const { return m_Indices; }

				private:
					int64_t m_Id{};
					std::string m_Name{};
					Array<Math::Float3> m_Points{};
					Array<Math::Float3> m_Normals{};
					Array<Math::Float2> m_Uvs{};
					Array<int> m_Indices{};

					void LoadPoints(const Reading::FbxObject& geometryObject);
					void LoadNormals(const Reading::FbxObject& geometryObject);
					void LoadIndices(Reading::FbxObject& geometryObject);
					void LoadUvs(const Reading::FbxObject& geometryObject);
				};
			}
		}
	}
}
