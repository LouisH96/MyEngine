#pragma once
#include <Math\Vectors.h>

namespace MyEngine
{
namespace MeshMaker
{

class MakerVertex
{
public:
	MakerVertex() = default;
	virtual ~MakerVertex() = default;
};

template<typename TVertex>
class MakerFullVertex
	: public MakerVertex
{
public:
	MakerFullVertex() = default;
	MakerFullVertex(const TVertex& vertex)
		: Vertex{ vertex }
	{};
	TVertex Vertex;
};

class MakerPointVertex
	: public MakerVertex
{
public:
	MakerPointVertex() = default;
	MakerPointVertex(const Float3& position)
		: MakerVertex{}
		, Position{ position }
	{};
	MakerPointVertex(float x, float y, float z)
		: MakerVertex{}
		, Position{ x,y,z }
	{};

	Float3 Position;
};

class MakerRefVertex
	: public MakerVertex
{
public:
	MakerRefVertex() = default;
	MakerRefVertex(unsigned index)
		: MakerVertex{}
		, Index{ index }
	{};

	unsigned Index;
};

}
}