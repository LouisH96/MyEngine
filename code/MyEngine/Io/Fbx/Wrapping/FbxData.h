#pragma once
#include "Geometry.h"

namespace MyEngine
{
	namespace Io
	{
		namespace Fbx
		{
			namespace Reading
			{
				class FbxObject;
				class FbxReader;
			}

			namespace Wrapping
			{
				class FbxData
				{
				public:
					FbxData(const std::wstring& fbxPath);
					FbxData(Reading::FbxReader&& reader);

					const Geometry& GetGeometry(int idx) const { return m_Geometries[idx]; }
					Geometry& GetGeometry(int idx) { return m_Geometries[idx]; }
					const Array<Geometry>& GetGeometries() const { return m_Geometries; }
					Array<Geometry>& GetGeometries() { return m_Geometries; }

				private:
					Array<Geometry> m_Geometries{};
				};
			}
		}
	}
}
