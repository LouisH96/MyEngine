#pragma once
#include <Math\Vectors.h>
#include <Applied\MeshMaker\MakerVertex.h>
#include <DataStructures\Pointers\SharedPtr.h>

namespace MyEngine
{
namespace MeshMaker
{
//1 normal only for now
class Arc
{
public:
	using Vertex = SharedPtr<const MakerVertex>;

	Arc() = default;

	void EnsureNrCorners(unsigned nrCorners);

	void AddCorner(Vertex corner);
	void SetCenter(Vertex center);
	void SetNormal(const Float3& normal);

	void AddCorner(const Float3& corner);
	void SetCenter(const Float3& center);

	void ClearCorners();

	Vertex GetCorner(unsigned i) const;
	const Float3& GetNormal() const;

	const List<Vertex>& GetCorners() const { return m_Corners; }
	Vertex GetCenter() const { return m_Center; }

	unsigned GetNrCorners() const { return m_Corners.GetSize(); }

private:
	List<Vertex> m_Corners; //clock-wise
	Vertex m_Center;

	Float3 m_Normal;
};
}
}