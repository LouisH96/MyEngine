#pragma once

#include "MakerData.h"
#include <DataStructures\Pointers\SharedPtr.h>
#include <Math\Vectors.h>

namespace MyEngine
{
namespace MeshMaker2
{

#pragma region MakerVertex

template<typename TVertex>
struct MakerVertexContent
{
public:
	MakerVertexContent() = default;
	virtual ~MakerVertexContent() = default;

	virtual TVertex& GetVertex(const MakerDataBase<TVertex, false>& data) = 0;
	virtual TVertex& GetVertex(const MakerDataBase<TVertex, true>& data) = 0;

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
	MakerVertex() = default;

	template<typename O>
	MakerVertex(const SharedPtr<O>& other) : SharedPtr<MakerVertexContent<TVertex>>{ other } {}
	template<typename O>
	MakerVertex(SharedPtr<O>&& other) : SharedPtr<MakerVertexContent<TVertex>>{ std::move(other) } {}

	TVertex& GetVertex(const MakerDataBase<TVertex, false>& data) { return Get().GetVertex(data); }
	TVertex& GetVertex(const MakerDataBase<TVertex, true>& data) { return Get().GetVertex(data); }

	template<bool THasIndices>
	void SetNormal(const Float3& normal, const MakerDataBase<TVertex, THasIndices>& data);

	TVertex MakeShapeVertex(MakerDataBase<TVertex, false>& data) { return Get().MakeShapeVertex(data); }
	int MakeShapeVertex(MakerDataBase<TVertex, true>& data) { return Get().MakeShapeVertex(data); }
};

template<typename TVertex>
template<bool THasIndices>
inline void MakerVertex<TVertex>::SetNormal(const Float3& normal, const MakerDataBase<TVertex, THasIndices>& data)
{
	GetVertex(data).Normal = normal;
}

#pragma endregion

#pragma region FullVertex

template<typename TVertex>
struct FullMakerVertexContent
	: public MakerVertexContent<TVertex>
{
public:
	TVertex Vertex{};

	TVertex& GetVertex(const MakerDataBase<TVertex, false>& data) override {
		return Vertex;
	}
	TVertex& GetVertex(const MakerDataBase<TVertex, true>& data) override {
		return Vertex;
	}

	TVertex MakeShapeVertex(MakerDataBase<TVertex, false>& data) override;
	int MakeShapeVertex(MakerDataBase<TVertex, true>& data) override;
};

template<typename TVertex>
inline TVertex FullMakerVertexContent<TVertex>::MakeShapeVertex(MakerDataBase<TVertex, false>& data)
{
	return Vertex;
}

template<typename TVertex>
inline int FullMakerVertexContent<TVertex>::MakeShapeVertex(MakerDataBase<TVertex, true>& data)
{
	return static_cast<int>(data.Vertices.Add(Vertex));
}

template<typename TVertex>
class FullMakerVertex final
	: public SharedPtr<FullMakerVertexContent<TVertex>>
{
private:
	using BaseClass = SharedPtr<FullMakerVertexContent<TVertex>>;
	using BaseClass::Get;

public:
	FullMakerVertex() = default;
	FullMakerVertex(const TVertex& vertex);
	FullMakerVertex(const Float3& position);
	FullMakerVertex(const Float3& position, const Float3& normal);

	TVertex& GetVertex() { return BaseClass::Get().Vertex; }
	const TVertex& GetVertex() const { return BaseClass::Get().Vertex; }

	void SetPosition(const Float3& newPosition) { GetVertex().Pos = newPosition; }
	Float3& GetPosition() { return GetVertex().Pos; }
	const Float3& GetPosition() const { return GetVertex().Pos; }

	void SetNormal(const Float3& newNormal) { GetVertex().Normal = newNormal; }
	Float3& GetNormal() { return GetVertex().Normal; }
	const Float3& GetNormal() const { return GetVertex().Normal; }
};

template<typename TVertex>
inline FullMakerVertex<TVertex>::FullMakerVertex(const TVertex& vertex)
	: SharedPtr<FullMakerVertexContent<TVertex>>{ vertex }
{
}
template<typename TVertex>
inline FullMakerVertex<TVertex>::FullMakerVertex(const Float3& position)
	: SharedPtr<FullMakerVertexContent<TVertex>>{ {} }
{
	SetPosition(position);
}
template<typename TVertex>
inline FullMakerVertex<TVertex>::FullMakerVertex(const Float3& position, const Float3& normal)
	: SharedPtr<FullMakerVertexContent<TVertex>>{ {} }
{
	SetPosition(position);
	SetNormal(normal);
}

#pragma endregion

#pragma region RefVertex

template<typename TVertex>
struct RefMakerVertexContent
	: public MakerVertexContent<TVertex>
{
public:
	int Index;

	TVertex GetVertex(const MakerDataBase<TVertex, false>& data) override {
		return data.Vertices[Index];
	}
	TVertex GetVertex(const MakerDataBase<TVertex, true>& data) override {
		return data.Vertices[Index];
	}

	TVertex MakeShapeVertex(MakerDataBase<TVertex, false>& data) override;
	int MakeShapeVertex(MakerDataBase<TVertex, true>& data) override;
};

template<typename TVertex>
inline TVertex RefMakerVertexContent<TVertex>::MakeShapeVertex(MakerDataBase<TVertex, false>& data)
{
	return data.Vertices[Index];
}

template<typename TVertex>
inline int RefMakerVertexContent<TVertex>::MakeShapeVertex(MakerDataBase<TVertex, true>& data)
{
	return Index;
}

template<typename TVertex>
class RefMakerVertex
	: public SharedPtr<RefMakerVertexContent<TVertex>>
{
private:
	using BaseClass = SharedPtr<RefMakerVertexContent<TVertex>>;
	using BaseClass::Get;

public:
	RefMakerVertex() = default;
	RefMakerVertex(int index);

	void SetIndex(int index);
	int GetIndex() const { return Get().Index; }
};

template<typename TVertex>
inline RefMakerVertex<TVertex>::RefMakerVertex(int index)
	: BaseClass{ index }
{};

template<typename TVertex>
inline void RefMakerVertex<TVertex>::SetIndex(int index)
{
	Get().Index = index;
}

#pragma endregion

}
}