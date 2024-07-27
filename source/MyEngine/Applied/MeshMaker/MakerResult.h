#pragma once

namespace MyEngine
{
namespace MeshMaker
{
class MakerResult
{
public:
	MakerResult() = default;
	MakerResult(unsigned capacity);

	void EnsureIncrease(unsigned increase);

	void Add(unsigned index);
	void Add(const MakerResult& other);
	void RemoveLast();

	template<typename Function, typename Vertex>
	void AdaptAll(Function function, List<Vertex>& vertices);

	const List<unsigned>& GetIndices() const { return m_Indices; }
	unsigned GetNrIndices() const { return m_Indices.GetSize(); }

private:
	List<unsigned> m_Indices{};

};

template<typename Function, typename Vertex>
inline void MakerResult::AdaptAll(Function function, List<Vertex>& vertices)
{
	for (unsigned i = 0; i < m_Indices.GetSize(); i++)
		function(vertices[m_Indices[i]]);
}
}
}