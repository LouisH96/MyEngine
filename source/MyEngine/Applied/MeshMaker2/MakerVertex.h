#pragma once

#include "MakerData.h"
#include <DataStructures\Pointers\SharedPtr.h>
#include <Math\Vectors.h>

namespace MyEngine
{
namespace MeshMaker2
{

template<typename TVertex>
struct FullMakerVertexContent;
template<typename TVertex>
struct RefMakerVertexContent;

template<typename TVertex>
struct MakerVertexContent
{
public:
	template<bool THasIndices>
	using Data = MakerDataBase<TVertex, THasIndices>;

	//---| Construction |---
	MakerVertexContent() = default;
	virtual ~MakerVertexContent() = default;

	//---| Vertex |---
	virtual TVertex& GetVertex(const MakerDataBase<TVertex, false>& data) = 0;
	virtual TVertex& GetVertex(const MakerDataBase<TVertex, true>& data) = 0;

	virtual const TVertex& GetVertex(const MakerDataBase<TVertex, false>& data) const = 0;
	virtual const TVertex& GetVertex(const MakerDataBase<TVertex, true>& data) const = 0;

	template<bool THasIndices>
	void TrySetPosition(const Float3& position, Data<THasIndices>& data) const {};
	template<bool THasIndices>
	void TrySetPosition(const Float3& position, Data<THasIndices>& data) { GetVertex(data).Pos = position; };

	template<bool THasIndices>
	void TrySetNormal(const Float3& normal, Data<THasIndices>& data) const {};
	template<bool THasIndices>
	void TrySetNormal(const Float3& normal, Data<THasIndices>& data) { GetVertex(data).Normal = normal; }

	//---| ShapeVertex |---
	virtual TVertex MakeShapeVertex(MakerDataBase<TVertex, false>& data) = 0;
	virtual int MakeShapeVertex(MakerDataBase<TVertex, true>& data) = 0;
};

template<typename TVertex>
class MakerVertex
	: public SharedPtr<MakerVertexContent<TVertex>>
{
private:
	using BaseClass = SharedPtr<MakerVertexContent<TVertex>>;
	using BaseClass::Get;

public:
	template<bool THasIndices>
	using Data = MakerDataBase<TVertex, THasIndices>;

	//---| Construction |---
	MakerVertex() = default;

	template<typename O>
	MakerVertex(const SharedPtr<O>& other) : SharedPtr<MakerVertexContent<TVertex>>{ other } {}
	template<typename O>
	MakerVertex(SharedPtr<O>&& other) : SharedPtr<MakerVertexContent<TVertex>>{ std::move(other) } {}

	template<template<typename> typename O>
	MakerVertex(const O<TVertex>& content) : SharedPtr<MakerVertexContent<TVertex>>{ content } {}

	//---| Vertex |---
	template<bool THasIndices>
	const TVertex& GetVertex(const Data<THasIndices>& data) const { return Get().GetVertex(data); }

	template<bool THasIndices>
	const Float3& GetPosition(const Data<THasIndices>& data) const { return Get().GetPosition(data); }
	template<bool THasIndices>
	const Float3& GetNormal(const Data<THasIndices>& data) const { return Get().GetNormal(data); }

	template<bool THasIndices>
	void TrySetPosition(const Float3& position, Data<THasIndices>& data) { Get().TrySetPosition(position, data); }
	template<bool THasIndices>
	void TrySetPosition(const Float3& position, Data<THasIndices>& data) const { Get().TrySetPosition(position, data); }

	template<bool THasIndices>
	void TrySetNormal(const Float3& normal, Data<THasIndices>& data) { Get().TrySetNormal(normal, data); }
	template<bool THasIndices>
	void TrySetNormal(const Float3& normal, Data<THasIndices>& data) const { Get().TrySetNormal(normal, data); }

	//---| ShapeVertex |---
	TVertex MakeShapeVertex(MakerDataBase<TVertex, false>& data) { return Get().MakeShapeVertex(data); }
	int MakeShapeVertex(MakerDataBase<TVertex, true>& data) { return Get().MakeShapeVertex(data); }
};

template<typename TVertex>
struct FullMakerVertexContent
	: public MakerVertexContent<TVertex>
{
public:
	FullMakerVertexContent() = default;
	FullMakerVertexContent(TVertex vertex) : Vertex{ vertex } {};

	TVertex Vertex{};

	TVertex& GetVertex(const MakerDataBase<TVertex, false>& data) override {
		return Vertex;
	}
	TVertex& GetVertex(const MakerDataBase<TVertex, true>& data) override {
		return Vertex;
	}
	const TVertex& GetVertex(const MakerDataBase<TVertex, false>& data) const override {
		return Vertex;
	}
	const TVertex& GetVertex(const MakerDataBase<TVertex, true>& data) const override {
		return Vertex;
	}

	TVertex MakeShapeVertex(MakerDataBase<TVertex, false>& data) override { return Vertex; };
	int MakeShapeVertex(MakerDataBase<TVertex, true>& data) override { return static_cast<int>(data.Vertices.Add(Vertex)); }
};

template<typename TVertex>
struct RefMakerVertexContent
	: public MakerVertexContent<TVertex>
{
public:
	int Index;

	TVertex& GetVertex(const MakerDataBase<TVertex, false>& data) override {
		return data.Vertices[Index];
	}
	TVertex& GetVertex(const MakerDataBase<TVertex, true>& data) override {
		return data.Vertices[Index];
	}

	const TVertex& GetVertex(const MakerDataBase<TVertex, false>& data) const override {
		return data.Vertices[Index];
	}
	const TVertex& GetVertex(const MakerDataBase<TVertex, true>& data) const override {
		return data.Vertices[Index];
	}

	TVertex MakeShapeVertex(MakerDataBase<TVertex, false>& data) override { return data.Vertices[Index]; }
	int MakeShapeVertex(MakerDataBase<TVertex, true>& data) override { return Index; }
};


template<typename TVertex>
class MakeVertex
{
public:
	static MakerVertex<TVertex> Full(const TVertex& vertex);
	static MakerVertex<TVertex> Full(const Float3& position);
	static MakerVertex<TVertex> Full(const Float3& position, const Float3& normal);
	static MakerVertex<TVertex> ConstFull(const TVertex& vertex);
	static MakerVertex<TVertex> ConstFull(const Float3& position, const Float3& normal);

	static MakerVertex<TVertex> Ref(int index);
	static MakerVertex<TVertex> ConstRef(int index);
};

template<typename TVertex>
inline MakerVertex<TVertex> MakeVertex<TVertex>::Full(const TVertex& vertex)
{
	return MakerVertex<TVertex>{ FullMakerVertexContent<TVertex>{vertex} };
}

template<typename TVertex>
inline MakerVertex<TVertex> MakeVertex<TVertex>::Full(const Float3& position)
{
	TVertex vertex{};
	vertex.Pos = position;
	return Full(vertex);
}

template<typename TVertex>
inline MakerVertex<TVertex> MakeVertex<TVertex>::Full(const Float3& position, const Float3& normal)
{
	TVertex vertex{};
	vertex.Pos = position;
	vertex.Normal = normal;
	return Full(vertex);
}

template<typename TVertex>
inline MakerVertex<TVertex> MakeVertex<TVertex>::ConstFull(const TVertex& vertex)
{
	return SharedPtr<const FullMakerVertexContent<TVertex>>{ vertex };
}

template<typename TVertex>
inline MakerVertex<TVertex> MakeVertex<TVertex>::ConstFull(const Float3& position, const Float3& normal)
{
	TVertex vertex{};
	vertex.Pos = position;
	vertex.Normal = normal;
	return ConstFull(vertex);
}

template<typename TVertex>
inline MakerVertex<TVertex> MakeVertex<TVertex>::Ref(int index)
{
	return MakerVertex<TVertex>{ RefMakerVertexContent<TVertex>{ index } };
}

template<typename TVertex>
inline MakerVertex<TVertex> MakeVertex<TVertex>::ConstRef(int index)
{
	return SharedPtr<const RefMakerVertexContent<TVertex>>{ index };
}

}
}