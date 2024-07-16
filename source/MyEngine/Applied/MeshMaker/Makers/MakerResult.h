#pragma once

namespace MyEngine
{
namespace MeshMaker
{
template<unsigned C>
class MakerResult
{
public:
	static constexpr unsigned Count{ C };
	unsigned Indices[C]{};

	template<typename Function, typename Vertex>
	void AdaptAll(Function function, List<Vertex>& vertices);
};

template<unsigned C>
template<typename Function, typename Vertex>
inline void MakerResult<C>::AdaptAll(Function function, List<Vertex>& vertices)
{
	for (unsigned i = 0; i < C; i++)
		function(vertices[Indices[i]]);
}
}
}