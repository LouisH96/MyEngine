#pragma once
#include "Math/Float2.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxObject;
			class FbxReader;

			class FbxData
			{
			public:
				FbxData(const std::wstring& fbxPath);
				FbxData(FbxReader&& reader);

				const Array<Math::Float3>& GetPoints() const { return m_Points; }
				const Array<Math::Float3>& GetNormals() const { return m_Normals; }
				const Array<int>& GetIndices() const { return m_Indices; }
				const Array<Math::Float2>& GetUvs() const { return m_Uvs; }

				Array<Math::Float3>& GetPoints() { return m_Points; }
				Array<Math::Float3>& GetNormals() { return m_Normals; }
				Array<int>& GetIndices() { return m_Indices; }
				Array<Math::Float2>& GetUvs() { return m_Uvs; }

			private:
				Array<Math::Float3> m_Points{};
				Array<Math::Float3> m_Normals{};
				Array<Math::Float2> m_Uvs{};
				Array<int> m_Indices{};

				void MakeTriangleList();

				void LoadUvs(FbxObject& geometry);
			};
		}
	}
}
