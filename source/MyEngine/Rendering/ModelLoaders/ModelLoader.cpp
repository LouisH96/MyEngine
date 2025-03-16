#include "pch.h"
#include "ModelLoader.h"

#include <Io\Fbx\FbxClass.h>
#include <Io\Fbx\Wrapping\Geometry.h>

using namespace Rendering;
using namespace Io::Fbx;

void ModelLoader::Load(const std::wstring& path, float scale,
	Buffer<V_PosColNorm>& vertices, const Float3& color)
{
	//load
	FbxClass fbx{ path, scale };

	//count
	Array<FbxClass::Geometry>& geometries{ fbx.GetGeometries() };
	unsigned nrVertices{ 0 };
	for (unsigned i = 0; i < geometries.GetSize(); i++)
		nrVertices += geometries[i].Points.GetSize();

	//vertices
	Array<V_PosColNorm> tempVertices{ nrVertices };
	V_PosColNorm* pTempVertex{ &tempVertices[0] };

	for (unsigned iGeom = 0; iGeom < geometries.GetSize(); iGeom++)
	{
		FbxClass::Geometry& geometry{ geometries[iGeom] };
		for (unsigned iPoint = 0; iPoint < geometry.Points.GetSize(); iPoint++)
		{
			pTempVertex->Pos = geometry.Points[iPoint];
			pTempVertex->Normal = geometry.Normals[iPoint];
			pTempVertex->Color = color;
			pTempVertex++;
		}
	}
	vertices = Buffer<V_PosColNorm>{ {tempVertices}, false };
}

void ModelLoader::Load(const std::wstring& path, float scale,
	MeshData<V_PosNorUv, ModelTopology::TriangleList>& vertices)
{
	FbxClass fbx{ path, scale };

	for (unsigned iGeom{ 0 }; iGeom < fbx.GetGeometries().GetSize(); ++iGeom)
	{
		vertices.StartShape();
		FbxClass::Geometry& geometry{ fbx.GetGeometry(iGeom) };

		for (unsigned iPoint{ 0 }; iPoint < geometry.Points.GetSize(); ++iPoint)
		{
			V_PosNorUv vertex{};
			vertex.Pos = geometry.Points[iPoint];
			vertex.Normal = geometry.Normals[iPoint];
			vertex.Uv = geometry.Uvs[iPoint];
			vertices.Vertices.Add(vertex);
		}
	}
}
