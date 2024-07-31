#pragma once

#include <DataStructures\Dictionary.h>
#include <Rendering\Mesh\MeshData.h>

namespace MyEngine
{
namespace MeshMaker
{

#define TEMP_DEF template<typename TVertex, ModelTopology TTopology>
#define TEMP_ARG TVertex, TTopology

#pragma region MakerResultBase1

TEMP_DEF
class MakerResultBase1
{
public:
	template<typename Function>
	void AdaptAll(Function function, MeshData<TVertex, TTopology>& meshData);
	void Move(const Float3& movement, MeshData<TVertex, TTopology>& meshData);

protected:
	unsigned m_VertexBegin{ Uint::MAX };
	unsigned m_VertexEnd{ Uint::MAX };
};

TEMP_DEF
template<typename Function>
void MakerResultBase1<TEMP_ARG>::AdaptAll(
	Function function, MeshData<TVertex, TTopology>& meshData)
{
	for (unsigned i{ m_VertexBegin }; i < m_VertexEnd; ++i)
		function(meshData.Vertices[i]);
}

TEMP_DEF
void MakerResultBase1<TEMP_ARG>::Move(const Float3& movement, MeshData<TVertex, TTopology>& meshData)
{
	for (unsigned i{ m_VertexBegin }; i < m_VertexEnd; ++i)
		meshData.Vertices[i].Pos += movement;
}

#pragma endregion

#pragma region MakerResultBase2

template<typename TVertex, ModelTopology TTopology, bool THasIndices>
class MakerResultBase2;

#pragma endregion

#pragma region MakerResultBase2 - Only Vertices 

#undef TEMP_ARG
#define TEMP_ARG TVertex, TTopology, false

TEMP_DEF
class MakerResultBase2<TEMP_ARG>
	: public MakerResultBase1<TVertex, TTopology>
{
public:
	void Begin(const MeshData<TVertex, TTopology>& meshData);
	void End(const MeshData<TVertex, TTopology>& meshData);

	MakerResultBase2 Duplicate(MeshData<TVertex, TTopology>& meshData) const;

private:
	using BaseClass = MakerResultBase1<TVertex, TTopology>;
	using BaseClass::m_VertexBegin;
	using BaseClass::m_VertexEnd;
};

TEMP_DEF
void MakerResultBase2<TEMP_ARG>::Begin(
	const MeshData<TVertex, TTopology>& meshData)
{
	m_VertexBegin = meshData.Vertices.GetSize();
}

TEMP_DEF
void MakerResultBase2<TEMP_ARG>::End(
	const MeshData<TVertex, TTopology>& meshData)
{
	m_VertexEnd = meshData.Vertices.GetSize();
}

TEMP_DEF
inline MakerResultBase2<TEMP_ARG> MakerResultBase2<TEMP_ARG>::Duplicate(MeshData<TVertex, TTopology>& meshData) const
{
	MakerResultBase2 newResult{};
	newResult.Begin(meshData);

	for (unsigned i{ m_VertexBegin }; i < m_VertexEnd; ++i)
		meshData.Vertices.Add(meshData.Vertices[i]);

	newResult.End(meshData);
	return newResult;
}

#pragma endregion

#pragma region MakerResultBase2 - With Indices

#undef TEMP_ARG
#define TEMP_ARG TVertex, TTopology, true

TEMP_DEF
class MakerResultBase2<TEMP_ARG>
	: public MakerResultBase1<TVertex, TTopology>
{
public:
	void Begin(const MeshData<TVertex, TTopology>& meshData);
	void End(const MeshData<TVertex, TTopology>& meshData);

	MakerResultBase2<TEMP_ARG> Duplicate(MeshData<TVertex, TTopology>& meshData) const;

private:
	using BaseClass = MakerResultBase1<TVertex, TTopology>;
	using BaseClass::m_VertexBegin;
	using BaseClass::m_VertexEnd;
	unsigned m_IndexBegin{ Uint::MAX };
	unsigned m_IndexEnd{ Uint::MAX };
};

TEMP_DEF
void MakerResultBase2<TEMP_ARG>::Begin(
	const MeshData<TVertex, TTopology>& meshData)
{
	m_VertexBegin = meshData.Vertices.GetSize();
	m_IndexBegin = meshData.Indices.GetSize();
}

TEMP_DEF
void MakerResultBase2<TEMP_ARG>::End(
	const MeshData<TVertex, TTopology>& meshData)
{
	m_VertexEnd = meshData.Vertices.GetSize();
	m_IndexEnd = meshData.Indices.GetSize();
}

TEMP_DEF
MakerResultBase2<TEMP_ARG> MakerResultBase2<TEMP_ARG>::Duplicate(MeshData<TVertex, TTopology>& meshData) const
{
	MakerResultBase2 newResult{};
	newResult.Begin(meshData);

	Dictionary<unsigned, unsigned> table{};

	for (unsigned iIndex{ m_IndexBegin }; iIndex < m_IndexEnd; ++iIndex)
	{
		const int oldIndex{ meshData.Indices[iIndex] };

		if (oldIndex == -1)
		{
			meshData.Indices.Add(-1);
			continue;
		}

		if (const unsigned* pNew{ table.Get(oldIndex) })
			meshData.Indices.Add(*pNew);
		else
		{
			const unsigned newIndex{ meshData.Vertices.GetSize() };
			table.Add(oldIndex, newIndex);
			meshData.Indices.Add(newIndex);
			meshData.Vertices.Add(meshData.Vertices[oldIndex]);
		}
	}

	newResult.End(meshData);
	return newResult;
}

#pragma endregion

#pragma region MakerResult

template<typename TVertex, ModelTopology TTopology>
class MakerResult
	: public MakerResultBase2<TVertex, TTopology, TopologyInfo::HasIndices(TTopology)>
{

};

#pragma endregion

#undef TEMP_ARG
#undef TEMP_DEF

}
}