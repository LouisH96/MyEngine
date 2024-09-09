#pragma once

#include "MakerData.h"

namespace MyEngine
{
namespace MeshMaker2
{

#pragma region MakerResult - Parts

template<typename TVertex>
class MakerResultVertices
{
public:
	unsigned VerticesBegin{ Uint::MAX };
	unsigned VerticesEnd{ Uint::MAX };

	void Begin(const MakerDataVertices<TVertex>& vertices) { VerticesBegin = vertices.GetSize(); }
	void End(const MakerDataVertices<TVertex>& vertices) { VerticesEnd = vertices.GetSize(); }

	unsigned GetSize() const { return VerticesEnd - VerticesBegin; }

	template<typename Function>
	void AdaptAll(Function function, MakerDataVertices<TVertex>& vertices) const;
};

template<typename TVertex>
template<typename Function>
inline void MakerResultVertices<TVertex>::AdaptAll(Function function, MakerDataVertices<TVertex>& vertices) const
{
	for (unsigned i{ VerticesBegin }; i < VerticesEnd; ++i)
		function(vertices[i]);
}

class MakerResultIndices
{
public:
	unsigned IndicesBegin{ Uint::MAX };
	unsigned IndicesEnd{ Uint::MAX };

	void Begin(const MakerDataIndices& indices) { IndicesBegin = indices.GetSize(); }
	void End(const MakerDataIndices& indices) { IndicesEnd = indices.GetSize(); }

	unsigned GetSize() const { return IndicesEnd - IndicesBegin; }
};

#pragma endregion

#pragma region MakerResult

template<typename TVertex, bool THasIndices>
class MakerResultBase;

template<typename TVertex, ModelTopology TTopology>
using MakerResult = MakerResultBase<TVertex, TopologyInfo::HasIndices(TTopology)>;

template<typename TVertex>
class MakerResultBase<TVertex, false>
{
public:
	using MakerDataType = MakerDataBase<TVertex, false>;

	void Begin(const MakerDataType& data) {
		m_Vertices.Begin(data.Vertices);
	}
	void End(const MakerDataType& data) {
		m_Vertices.End(data.Vertices);
	}

	unsigned GetNrVertices() const { return m_Vertices.GetSize(); }
	unsigned GetVerticesBegin() const { return m_Vertices.VerticesBegin; }
	unsigned GetVerticesEnd() const { return m_Vertices.VerticesEnd; }

	template<typename Function>
	void AdaptAll(Function function, MakerDataType& data) const {
		m_Vertices.AdaptAll(function, data.Vertices);
	}

private:
	MakerResultVertices<TVertex> m_Vertices{};
};

template<typename TVertex>
class MakerResultBase<TVertex, true>
{
public:
	using MakerDataType = MakerDataBase<TVertex, true>;

	void Begin(const MakerDataType& data) {
		m_Vertices.Begin(data.Vertices);
		m_Indices.Begin(data.Indices);
	}
	void End(const MakerDataType& data) {
		m_Vertices.End(data.Vertices);
		m_Indices.End(data.Indices);
	}

	unsigned GetNrVertices() const { return m_Vertices.GetSize(); }
	unsigned GetVerticesBegin() const { return m_Vertices.VerticesBegin; }
	unsigned GetVerticesEnd() const { return m_Vertices.VerticesEnd; }
	unsigned GetIndicesBegin() const { return m_Indices.IndicesBegin; }
	unsigned GetIndicesEnd() const { return m_Indices.IndicesEnd; }

	template<typename Function>
	void AdaptAll(Function function, MakerDataType& data) const {
		m_Vertices.AdaptAll(function, data.Vertices);
	}

private:
	MakerResultVertices<TVertex> m_Vertices{};
	MakerResultIndices m_Indices{};
};

#pragma endregion

}
}