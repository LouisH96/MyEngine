#pragma once
#include <Math\Vectors.h>

namespace MyEngine
{
namespace MeshMaker
{

struct MakerVertex
{
public:
	MakerVertex() = default;
	virtual ~MakerVertex() = default;
};

template<typename TVertex>
struct MakerFullVertex
	: public MakerVertex
{
	MakerFullVertex() = default;
	MakerFullVertex(const TVertex& vertex)
		: Vertex{ vertex }
	{};
	TVertex Vertex;
};

struct MakerPointVertex
	: public MakerVertex
{
	MakerPointVertex() = default;
	MakerPointVertex(const Float3& position)
		: Position{ position }
	{};

	Float3 Position;
};

struct MakerRefVertex
	: public MakerVertex
{
	MakerRefVertex() = default;
	MakerRefVertex(unsigned index)
		: Index{ index }
	{};

	unsigned Index;
};

}
}