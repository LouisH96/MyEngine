#pragma once

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			class FbxReader;

			class FbxData
			{
			public:
				FbxData(const std::wstring& fbxPath);
				FbxData(FbxReader&& reader);

				const Array<Math::Float3>& GetPoints() const { return m_Points; }
				const Array<Math::Float3>& GetNormals() const { return m_Normals; }
				const Array<int>& GetIndices() const { return m_Indices; }

			private:
				Array<Math::Float3> m_Points{};
				Array<Math::Float3> m_Normals{};
				Array<int> m_Indices{};

				void MakeTriangleList();
			};
		}
	}
}
