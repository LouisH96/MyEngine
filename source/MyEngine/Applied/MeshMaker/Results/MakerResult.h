#pragma once

#include <DataStructures\Dictionary.h>
#include <Rendering\Mesh\MeshData.h>

namespace MyEngine
{
namespace MeshMaker
{

template<typename TVertex, ModelTopology TTopology, bool THasIndices>
class Duplicator;

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

protected:
	Duplicator<TVertex, TTopology, false> GetDuplicator(MeshData<TVertex, TTopology>& meshData) const;

private:
	using BaseClass = MakerResultBase1<TVertex, TTopology>;
	using BaseClass::m_VertexBegin;
	using BaseClass::m_VertexEnd;

	friend class Duplicator<TVertex, TTopology, false>;
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
	return GetDuplicator(meshData).GetMakerResult();
}

TEMP_DEF
inline Duplicator<TVertex, TTopology, false> MakerResultBase2<TEMP_ARG>::GetDuplicator(MeshData<TVertex, TTopology>& meshData) const
{
	return Duplicator<TVertex, TTopology, false>(*this, meshData);
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

protected:
	Duplicator<TVertex, TTopology, true> GetDuplicator(MeshData<TVertex, TTopology>& meshData) const;

private:
	using BaseClass = MakerResultBase1<TVertex, TTopology>;
	using BaseClass::m_VertexBegin;
	using BaseClass::m_VertexEnd;
	unsigned m_IndexBegin{ Uint::MAX };
	unsigned m_IndexEnd{ Uint::MAX };

	friend class Duplicator<TVertex, TTopology, true>;
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
inline MakerResultBase2<TEMP_ARG> MakerResultBase2<TEMP_ARG>::Duplicate(MeshData<TVertex, TTopology>& meshData) const
{
	return GetDuplicator(meshData).GetMakerResult();
}

TEMP_DEF
inline Duplicator<TVertex, TTopology, true> MakerResultBase2<TEMP_ARG>::GetDuplicator(MeshData<TVertex, TTopology>& meshData) const
{
	return Duplicator<TVertex, TTopology, true>{*this, meshData};
}

#undef TEMP_DEF
#undef TEMP_ARG

#pragma endregion

#pragma region MakerResult

template<typename TVertex, ModelTopology TTopology>
class MakerResult
	: public MakerResultBase2<TVertex, TTopology, TopologyInfo::HasIndices(TTopology)>
{
public:
	using Duplicator = Duplicator<TVertex, TTopology, TopologyInfo::HasIndices(TTopology)>;
};


#pragma endregion

#pragma region Duplicator - Without Indices

#define TEMP_DEF template<typename TVertex, ModelTopology TTopology>
#define TEMP_ARG TVertex, TTopology, false

template<typename TVertex, ModelTopology TTopology>
class Duplicator<TVertex, TTopology, false>
{
public:
	Duplicator(const MakerResultBase2<TVertex, TTopology, false>& toDuplicate, MeshData<TVertex, TTopology>& meshData);

	unsigned GetUpdated(unsigned oldIndex) const;
	MakerResult<TVertex, TTopology>&& GetMakerResult() { return std::move(m_NewResult); }

private:
	MakerResult<TVertex, TTopology> m_NewResult;
	unsigned m_PrevBegin;
};

TEMP_DEF
inline Duplicator<TEMP_ARG>::Duplicator(const MakerResultBase2<TVertex, TTopology, false>& toDuplicate, MeshData<TVertex, TTopology>& meshData)
	: m_PrevBegin{ toDuplicate.m_VertexBegin }
{
	m_NewResult.Begin(meshData);

	for (unsigned i{ toDuplicate.m_VertexBegin }; i < toDuplicate.m_VertexEnd; ++i)
		meshData.Vertices.Add(meshData.Vertices[i]);

	m_NewResult.End(meshData);
}

TEMP_DEF
inline unsigned Duplicator<TEMP_ARG>::GetUpdated(unsigned oldIndex) const
{
	return m_NewResult.m_VertexBegin + (oldIndex - m_PrevBegin);
}

#undef TEMP_DEF
#undef TEMP_ARG

#pragma endregion

#pragma region Duplicator - With Indices

#define TEMP_DEF template<typename TVertex, ModelTopology TTopology>
#define TEMP_ARG TVertex, TTopology, true

template<typename TVertex, ModelTopology TTopology>
class Duplicator<TVertex, TTopology, true>
{
public:
	Duplicator(const MakerResultBase2<TVertex, TTopology, true>& toDuplicate, MeshData<TVertex, TTopology>& meshData);

	MakerResult<TVertex, TTopology>&& GetMakerResult() { return std::move(m_NewResult); }
	unsigned GetUpdated(unsigned oldIndex);

private:
	MakerResult<TVertex, TTopology> m_NewResult;
	Dictionary<unsigned, unsigned> m_UpdateTable;
};

TEMP_DEF
inline Duplicator<TEMP_ARG>::Duplicator(const MakerResultBase2<TVertex, TTopology, true>& toDuplicate, MeshData<TVertex, TTopology>& meshData)
	: m_NewResult{}
{
	m_NewResult.Begin(meshData);

	for (unsigned iIndex{ toDuplicate.m_IndexBegin }; iIndex < toDuplicate.m_IndexEnd; ++iIndex)
	{
		const int oldIndex{ meshData.Indices[iIndex] };

		if (oldIndex == -1)
		{
			meshData.Indices.Add(-1);
			continue;
		}

		if (const unsigned* pNew{ m_UpdateTable.Get(oldIndex) })
			meshData.Indices.Add(*pNew);
		else
		{
			const unsigned newIndex{ meshData.Vertices.GetSize() };
			m_UpdateTable.Add(oldIndex, newIndex);
			meshData.Indices.Add(newIndex);
			meshData.Vertices.Add(meshData.Vertices[oldIndex]);
		}
	}

	m_NewResult.End(meshData);
}

TEMP_DEF
inline unsigned Duplicator<TEMP_ARG>::GetUpdated(unsigned oldIndex)
{
	return *m_UpdateTable.Get(oldIndex);
}

#pragma endregion

#undef TEMP_ARG
#undef TEMP_DEF

}
}